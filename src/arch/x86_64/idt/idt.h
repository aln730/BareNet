#pragma once
#include <stdint.h>

#define IDT_ENTRIES 256

struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct IDTPtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

void idt_init(void);
