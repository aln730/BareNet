global idt_load

section .text
idt_load:
    lidt [rdi]
    ret
