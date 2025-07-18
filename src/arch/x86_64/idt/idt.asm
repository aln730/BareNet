.intel_syntax noprefix
.global idt_load
.text

idt_load:
    lidt [rdi]
    ret
