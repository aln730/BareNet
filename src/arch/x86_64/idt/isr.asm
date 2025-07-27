section .text
global isr_stub_table
extern isr_common_stub

%assign i 0
%rep 32
    global isr_stub_%+i
isr_stub_%+i:
    push rdi
    mov rdi, i              
    call isr_common_stub
    pop rdi
    iretq
%assign i i+1
%endrep

section .data
isr_stub_table:
%assign i 0
%rep 32
    dq isr_stub_%+i
%assign i i+1
%endrep
