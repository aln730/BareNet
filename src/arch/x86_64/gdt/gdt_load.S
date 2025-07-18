.global gdt_load
gdt_load:
    mov rax, rdi
    lgdt [rax]

    ; Reload segment registers
    mov ax, 0x10           ; data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to flush pipeline and load CS
    pushq $0x08            ; code segment selector
    lea rax, [rip + .flush]
    push rax
    lretq

.flush:
    ret
