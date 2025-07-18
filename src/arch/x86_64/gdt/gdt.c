#include "gdt.h"

static struct GDTEntry gdt[3];
static struct GDTPtr gdt_ptr;

static void set_gdt_entry(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[idx].limit_low    = limit & 0xFFFF;
    gdt[idx].base_low     = base & 0xFFFF;
    gdt[idx].base_middle  = (base >> 16) & 0xFF;
    gdt[idx].access       = access;
    gdt[idx].granularity  = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[idx].base_high    = (base >> 24) & 0xFF;
}

extern void gdt_load(struct GDTPtr*);

void gdt_init(void)
{
    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base  = (uint64_t)&gdt;

    set_gdt_entry(0, 0, 0, 0, 0);               // null descriptor
    set_gdt_entry(1, 0, 0, 0x9A, 0x20);         // code segment
    set_gdt_entry(2, 0, 0, 0x92, 0x00);         // data segment
    
    gdt_load(&gdt_ptr);
}
