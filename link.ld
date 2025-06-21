ENTRY(_start)

KERNEL_LMA = 0x100000;
KERNEL_VMA = 0xffffffff80000000;

SECTIONS
{
    . = KERNEL_LMA;

    .bootstrap : {
        KEEP(*(.bootstrap)) /* Your bootstrap.s code */
    }

    . = KERNEL_VMA;

    .text : AT(ADDR(.text) - KERNEL_VMA) {
        _code = .;
        *(.text)
        *(.rodata*)
        . = ALIGN(4096);
    }

    .data : AT(ADDR(.data) - KERNEL_VMA) {
        _data = .;
        *(.data)
        . = ALIGN(4096);
    }

    .eh_frame : AT(ADDR(.eh_frame) - KERNEL_VMA) {
        *(.eh_frame)
        . = ALIGN(4096);
    }

    .bss : AT(ADDR(.bss) - KERNEL_VMA) {
        _bss = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4096);
    }

    _end = .;

    /DISCARD/ : {
        *(.comment)
    }
}
