#include "gdt.h"


struct gdt_entry{
    // GDT entry struct (packed)
}

struct gdt_ptr{
    // GDTR struct
}

// GDT table with 3 entries: null, code, data
static struct gdt_entry gdt[3];
static struct gdt_ptr gdtr;

extern void load_gdt(struct gdt_ptr* gdtr);

void gdt_set_entry(){

}

void gdt_init(void) {
    // TODO: Define GDT entries and load GDT
}
