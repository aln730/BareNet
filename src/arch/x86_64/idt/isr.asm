.intel_syntax noprefix
.global isr_stub_0
.text

isr_stub_0:
    cli
    hlt
