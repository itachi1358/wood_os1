bits 32

section .text
align 4

; Multiboot header
multiboot_header:
    dd 0x1BADB002        ; magic number
    dd 0x00000003        ; flags
    dd -(0x1BADB002 + 0x00000003)  ; checksum

global _start
extern kernel_main

_start:
    ; Set up stack
    mov esp, stack_top
    
    ; Call kernel
    call kernel_main
    
    ; Hang
    cli
hang:
    hlt
    jmp hang

section .bss
align 16
stack_bottom:
    resb 16384
stack_top: