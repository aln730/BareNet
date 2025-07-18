section .text
global idt_load


idt_load:
    mov rax, [rdi]
    lidt [rdi]
    ret



    