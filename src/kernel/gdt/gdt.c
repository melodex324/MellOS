#include <gdt.h>
#include <stmol.h>

extern void gdt_flush(uint64_t);
extern void tss_flush();

struct gdt_entry_struct gdt_entries[7];
struct gdt_pointer_struct gdt_pointer;

struct tss_entry_struct tss_entry;

void gdt_init()
{
    gdt_pointer.limit = (sizeof(struct gdt_entry_struct) * 7) - 1;
    gdt_pointer.base = (uint64_t)&gdt_entries;

    gdt_gate(0, 0, 0, 0, 0);                        //null segment
    gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xA0);         //Kernel code segment
    gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xC0);         //Kernel data segment
    gdt_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xA0);         //User code segment
    gdt_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xC0);         //User data segment

    memset(&tss_entry, 0, sizeof(tss_entry));
    tss_entry.iomap_base = sizeof(tss_entry);

    static uint8_t kernel_stack[16384] __attribute__((aligned(16)));
    tss_entry.rsp0 = (uint64_t)&kernel_stack[sizeof(kernel_stack)];

    tss_gate(5, (uint64_t)&tss_entry, sizeof(tss_entry) - 1);                       //Task state segment

    gdt_flush((uint64_t)&gdt_pointer);
    tss_flush();

    write_string("GDT Initiated ");
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

void tss_gate(uint32_t num, uint64_t base, uint32_t limit)
{
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_mid = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit = (limit & 0xFFFF);
    gdt_entries[num].flags = ((limit >> 16) & 0x0F);
    gdt_entries[num].acces_byte = 0x89;

    uint64_t* tss_high = (uint64_t*)&gdt_entries[num + 1];
    *tss_high = (base >> 32);
}
