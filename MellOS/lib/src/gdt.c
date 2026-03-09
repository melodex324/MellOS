#include <stdint.h>
#include <stmol.h>
#include <gdt.h>

extern void gdt_flush(uint32_t);

struct gdt_entry_struct gdt_entries[5];
struct gdt_pointer_struct gdt_pointer;

void gdt_init()
{
    gdt_pointer.limit = (sizeof(struct gdt_entry_struct) * 5) - 1;
    gdt_pointer.base = (uint32_t)&gdt_entries;

    gdt_gate(0,0,0,0,0);                        //null segment
    gdt_gate(1,0,0xFFFFFFFF,0x9A,0xCF);         //Kernel code segment
    gdt_gate(2,0,0xFFFFFFFF,0x92,0xCF);         //Kernel data segment
    gdt_gate(3,0,0xFFFFFFFF,0xFA,0xCF);         //User code segment
    gdt_gate(4,0,0xFFFFFFFF,0xF2,0xCF);         //User data segment
    
    gdt_flush((uint32_t)&gdt_pointer);

    terminal_writestring("GDT initated ");

}

void gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t acces, uint8_t gran)
{
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_mid = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit = (limit & 0xFFFF);
    gdt_entries[num].flags = (limit >> 16) & 0x0F;
    gdt_entries[num].flags |= (gran & 0xF0);

    gdt_entries[num].acces_byte = acces;
}