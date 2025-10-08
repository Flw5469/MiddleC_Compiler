section .data
    msg_format db "EAP register's value = ", 0
    newline db 10, 0
    digits db "0123456789", 0

section .bss
    heap_space resb 1024    ; Our static "heap" - 1024 bytes
    heap_ptr resd 1         ; Pointer to next free space
    number_str resb 12      ; Buffer for number-to-string conversion

section .text
global _start

; Simple malloc using static allocation
malloc:
    push ebp
    mov ebp, esp
    
    ; Initialize heap pointer if first call
    cmp dword [heap_ptr], 0
    jne heap_initialized
    mov dword [heap_ptr], heap_space
    
heap_initialized:
    mov eax, [heap_ptr]     ; Get current heap position
    mov ebx, [ebp+8]        ; Get size parameter from stack
    add [heap_ptr], ebx     ; Advance heap pointer
    
    pop ebp
    ret 4                   ; Clean up stack (remove size parameter)

; Simple free (does nothing in this implementation)
free:
    push ebp
    mov ebp, esp
    pop ebp
    ret 4

; Convert number in eax to string in number_str
; Returns: eax = pointer to string
number_to_string:
    push ebp
    mov ebp, esp
    push ebx
    push ecx
    push edx
    push edi
    
    mov edi, number_str + 11    ; Point to end of buffer
    mov byte [edi], 0           ; Null terminate
    dec edi
    mov ecx, 10                 ; Base 10
    
    ; Handle zero case
    test eax, eax
    jnz convert_loop
    mov byte [edi], '0'
    mov eax, edi                ; Return pointer to '0' directly!
    jmp done_zero               ; Skip the inc edi
    
convert_loop:
    xor edx, edx
    div ecx                     ; Divide by 10
    add dl, '0'                 ; Convert remainder to ASCII
    mov [edi], dl
    dec edi
    test eax, eax
    jnz convert_loop
    
convert_done:
    inc edi                     ; Point to first digit
    mov eax, edi                ; Return pointer to string
    
done_zero:
    pop edi
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret

; Print null-terminated string
; Input: string address on stack
print_string:
    push ebp
    mov ebp, esp
    push ebx
    push ecx
    push edx
    
    mov ecx, [ebp+8]    ; String address
    mov edx, 0          ; Calculate length
    mov ebx, ecx
    
strlen_loop:
    cmp byte [ebx], 0
    je strlen_done
    inc ebx
    inc edx
    jmp strlen_loop
    
strlen_done:
    ; Write system call
    mov eax, 4          ; sys_write
    mov ebx, 1          ; stdout
    int 0x80
    
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret 4

; Function: show
; Parameters: eax = number to display
show_function:
    push ebp
    mov ebp, esp
    push eax            ; Save the number
    push eax

    ; Print "EAP register's value = "
    push msg_format
    call print_string
    
    ; Convert number to string and print it
    pop eax             ; Restore the number
    call number_to_string
    push eax
    call print_string
    
    ; Print newline
    push newline
    call print_string
    
    pop eax
    pop ebp
    ret

_start:
    mov ebp, esp    
    ; var peter 
    push 0
    mov eax, ebp ; access the stack frame
    mov eax, [eax-4]; load the variable into eax
    
    ; peter = malloc 100 
    mov eax, ebp ; access the stack frame
    mov eax, [eax-4]; load the variable into eax
    mov eax, 100
    push eax
    call malloc
    mov ebx, eax
    mov eax, ebp; access the stack frame
    mov dword [eax-4], ebx; store ebx into memory
    
    ; ( peter ) = 20 
    mov eax, ebp ; access the stack frame
    mov eax, [eax-4]; load the variable into eax
    push eax
    mov eax, 20
    mov ebx, eax
    pop eax
    mov dword [eax], ebx
    
    ; if ( ( * peter ) / 5 - 4 ) { function:show ; } 
    mov eax, ebp ; access the stack frame
    mov eax, [eax-4]; load the variable into eax
    mov eax, [eax]      ; dereference the address
    push eax
    mov eax, 5
    mov ebx, eax
    pop eax
    cdq
    idiv ebx
    push eax
    mov eax, 4
    mov ebx, eax
    pop eax
    sub eax, ebx
    cmp eax, 0             ; compare with 0
    je if_end_1
    push ebp          ; Save caller's base pointer
    mov ebp, esp      ; Set up new base pointer
    call show_function
    mov esp, ebp      ; Restore stack pointer
    pop ebp           ; Restore caller's base pointer
    if_end_1:
    call show_function
    
    ; Exit program
    mov eax, 1                  ; sys_exit
    mov ebx, 0                  ; exit status
    int 0x80