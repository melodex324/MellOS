[bits 64]
global _start
extern kernel_main

section .text
_start:
    cli
    mov rbp, 0
    and rsp, -16

    call kernel_main

.hang:
    cli
    hlt
    jmp .hang

section .note.GNU-stack noalloc noexec nowrite progbits