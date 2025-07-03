#include "gdt.h"

// GDT entry struct (packed)
struct gdt_entry{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// GDTR struct
struct gdt_ptr{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

// GDT table with 3 entries: null, code, data
static struct gdt_entry gdt[3];
static struct gdt_ptr gdtr;

extern void load_gdt(struct gdt_ptr* gdtr);

void gdt_set_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[i].limit_low = limit & 0xFFFF;
    gdt[i].base_low = base & 0xFFFF;
    gdt[i].base_mid = (base >> 16) & 0xFF;
    gdt[i].access = access;
    gdt[i].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[i].base_high = (base >> 24) & 0xFF;
}

void gdt_init(void) {
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (uint64_t)&gdt;

    gdt_set_entry(0, 0, 0, 0, 0);                // Null
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xA0);    // Code
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xA0);    // Data

    load_gdt(&gdtr);
}
