#ifndef GDT_H
#define GDT_H
#include <stdint.h>

struct gdt_entry_struct
{
    uint16_t limit;         //limit --> 0-15
    uint16_t base_low;      // -->
    uint8_t base_mid;       // -->
    uint8_t acces_byte;     // -->
    uint8_t flags;          // -->
    uint8_t base_high;      // -->
}__attribute__((packed));   // -->

struct gdt_pointer_struct
{
    uint16_t limit;
    unsigned int base;
}__attribute__((packed));

void gdt_init();
void gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t acces, uint8_t gran);

#endif