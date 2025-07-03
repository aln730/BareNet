section .text
global load_gdt

load_gdt:
    mov rax, [rdi]
    lgdt [rdi]             ; Load GDTR
    mov ax, 0x10           ; Data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush         ; Code selector, far jump

flush:
    ret
