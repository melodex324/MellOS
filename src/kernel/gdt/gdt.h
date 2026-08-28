#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_entry_struct
{
    uint16_t limit;         // 2 octects
    uint16_t base_low;      // 2 octects
    uint8_t base_mid;       // 1 octects
    uint8_t acces_byte;     // 1 octects
    uint8_t flags;          // 1 octects
    uint8_t base_high;      // 1 octects
}__attribute__((packed));   // 2 + 2 + (4*1) = 8 

struct gdt_pointer_struct
{
    uint16_t limit;
    uint64_t base;
}__attribute__((packed));

struct tss_entry_struct
{
    uint32_t reserved0;     // 4 octects
    uint64_t rsp0;          // 8 octects
    uint64_t rsp1;          // 8 octects
    uint64_t rsp2;          // 8 octects
    uint64_t reserved1;     // 8 octects
    uint64_t ist1;          // 8 octects
    uint64_t ist2;          // 8 octects
    uint64_t ist3;          // 8 octects
    uint64_t ist4;          // 8 octects
    uint64_t ist5;          // 8 octects
    uint64_t ist6;          // 8 octects
    uint64_t ist7;          // 8 octects
    uint64_t reserved2;     // 8 octects
    uint16_t reserved3;     // 2 octects
    uint16_t iomap_base;    // 2 octects
}__attribute__((packed));   // 4 + (8*12) + (2*2) = 104


extern void gdt_init();
extern void gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t acces, uint8_t gran);
extern void tss_gate(uint32_t num, uint64_t base, uint32_t limit);

#endif