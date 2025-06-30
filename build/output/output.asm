; NASM syntax
bits 32
section .data
    msg_format db "EAP register's value = %d", 0    ; Format string with placeholder
    message db 32 dup(0)              ; Buffer for our formatted message
    title db "Assembly Calculation", 0

section .text
global start
extern _MessageBoxA@16               ; stdcall - decorated
extern wsprintfA                     ; cdecl - no decoration
extern _ExitProcess@4                ; stdcall - decorated
extern _GetProcessHeap@0
extern _HeapAlloc@12
extern _HeapFree@12

; Function: show
; Parameters: eax = number to display
; Uses cdecl calling convention
show_function:
    push ebp            ; Save caller's frame pointer
    mov ebp, esp        ; Set up our frame pointer
    
    ; Format the result string using wsprintfA (cdecl calling convention)
    push eax            ; Push the result (13)
    push msg_format     ; Push format string
    push message        ; Push destination buffer
    call wsprintfA      ; Call wsprintfA
    add esp, 12         ; Clean up stack (caller cleans for cdecl)
    
    ; Show message box with the result
    push 0              ; MB_OK
    push title          ; Title
    push message        ; Our formatted message
    push 0              ; hWnd
    call _MessageBoxA@16
    
    mov esp, ebp        ; Restore stack pointer
    pop ebp             ; Restore caller's frame pointer
    ret                 ; Return to caller

; Allocate heap memory
; Input: element in stack
; Output: eax = pointer to allocated memory (or 0 if failed)
malloc:
    push ebp
    mov ebp, esp
    push ebx            ; Save ebx
    
    mov ebx, [ebp+8]    ; Get size parameter from stack
    
    call _GetProcessHeap@0
    push ebx            ; Push size
    push 0              ; Push flags (HEAP_NO_SERIALIZE could be 0x1)
    push eax            ; Push heap handle
    call _HeapAlloc@12  ; Allocate memory
    
    pop ebx             ; Restore ebx
    pop ebp
    ret 4               ; Clean up 4 bytes (size parameter)

; Free heap memory  
; Input: element in stack as the heap memory location
free:
    push ebp
    mov ebp, esp
    push ebx            ; Save ebx
    
    mov ebx, [ebp+8]    ; Get pointer parameter from stack
    
    call _GetProcessHeap@0  ; Get heap handle in eax
    push ebx            ; Push pointer to free
    push 0              ; Push flags  
    push eax            ; Push heap handle
    call _HeapFree@12   ; Free memory
    
    pop ebx             ; Restore ebx
    pop ebp
    ret 4               ; Clean up pointer parameter

start:
    ; Perform the calculation
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
    
    ; if ( ( * peter ) / 5 ) { function:show ; } 
    mov eax, ebp ; access the stack frame
    mov eax, [eax-4]; load the variable into eax
    mov eax, [eax]      ; dereference the address
    push eax
    mov eax, 5
    mov ebx, eax
    pop eax
    cdq
    idiv ebx
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
    push 0              ; Exit code
    call _ExitProcess@4