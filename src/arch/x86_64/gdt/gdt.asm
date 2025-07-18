; src/gdt.asm
global gdt_load

section .text
bits 64

gdt_load:
    lgdt [rdi]    
    ret
