#include "idt.h"
#include <stddef.h> 
extern void idt_load(idtr_t*);

static idt_entry_t idt[IDT_ENTRIES] __attribute__((aligned(16)));
static idtr_t idtr;

extern void isr_stub_0(void);

static void idt_set_entry(int vec, void* isr, uint8_t flags) {
    uint64_t addr = (uint64_t)isr;
    idt[vec].offset_low  = addr & 0xFFFF;
    idt[vec].selector    = 0x08;      
    idt[vec].ist         = 0;
    idt[vec].type_attr   = flags;
    idt[vec].offset_mid  = (addr >> 16) & 0xFFFF;
    idt[vec].offset_high = (addr >> 32) & 0xFFFFFFFF;
    idt[vec].zero        = 0;
}

void idt_init(void) {
    for (size_t i = 0; i < IDT_ENTRIES; i++)
        idt[i] = (idt_entry_t){0};

    idt_set_entry(0, isr_stub_0, 0x8E);

    idtr.limit = sizeof(idt) - 1;
    idtr.base  = (uint64_t)&idt;
    idt_load(&idtr);
    __asm__ volatile("sti");
}
