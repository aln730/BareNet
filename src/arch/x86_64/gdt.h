#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;  
    uint8_t base_middle; 
    uint8_t access;
    uint16_t granularity;
    uint8_t base_high;
} __attribute__((packed));


struct GDTPtr {
    uint16_t limit;         
    uint64_t base;          
} __attribute__((packed));

void gdt_init(void);

#endif