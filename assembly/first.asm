; NASM syntax
bits 32
section .data
    msg_format db "The final answer = %d", 0    ; Format string with placeholder
    message db 32 dup(0)              ; Buffer for our formatted message
    title db "Assembly Calculation", 0

section .text
global start
extern _MessageBoxA@16               ; stdcall - decorated
extern wsprintfA                     ; cdecl - no decoration
extern _ExitProcess@4                ; stdcall - decorated

start:
    ; Perform the calculation
mov eax, 6
add eax, 4
mov ebx, 2
xor edx, edx
idiv ebx
add eax, 1
mov ebx, 1
xor edx, edx
idiv ebx

    
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


    ; Exit program
    push 0              ; Exit code
    call _ExitProcess@4