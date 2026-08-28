#include <gdt.h>
#include <display.h>
#include <stmol.h>
#include <terminal.h>
#include <io.h>

#include <idt.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

extern void* isr_stub_table[32];
extern void* irq_stub_table[16];
extern void idt_flush(uint64_t);

struct idt_entry_struct idt_entries[256];
struct idt_pointer_struct idt_pointer;

static irq_handler_t irq_routines[16] = { 0 };

void idt_init(void)
{
    idt_pointer.limit = (sizeof(struct idt_entry_struct) * 256) - 1;
    idt_pointer.base = (uint64_t)&idt_entries;

    pic_remap(32, 40);

    for (size_t i = 0; i < 32; i++)
    {
        idt_gate(i, (uint64_t)isr_stub_table[i], 0x08, 0x8E, 0);
    }

    for (size_t i = 0; i < 16; i++)
    {
        idt_gate(i + 32, (uint64_t)irq_stub_table[i], 0x08, 0x8E, 0);
    }
    
    idt_flush((uint64_t)&idt_pointer);
    __asm__ __volatile__("sti");

    write_string("IDT Initiated ");
}

void idt_gate(uint8_t num, uint64_t handler, uint16_t selector, uint8_t flags, uint8_t ist)
{
    idt_entries[num].offset1 = (uint64_t)(handler & 0xFFFF);
    idt_entries[num].selector = selector;
    idt_entries[num].ist = ist & 0x07;
    idt_entries[num].gate_type = flags;
    idt_entries[num].offset2 = (uint16_t)((handler >> 16) & 0xFFFF);
    idt_entries[num].offset3 = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
    idt_entries[num].reserved = 0;
}

const char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "SIM FP Exception",
    "Virtualization Exception",
    "Control Protection",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection",
    "VMM Communication",
    "Security Exception",
    "Reserved"
};

void isr_handler(registers_t* regs)
{
    if (regs->int_no < 32)
    {
        for (size_t y = 0; y < fb->height; y++)
        {
            for (size_t x = 0; x < fb->width; x++)
            {
                fb_ptr[y * stride + x] = kernel_blue;
            }
        }

        terminal_row = (TERMINAL_HEIGHT / 2);
        terminal_column = (TERMINAL_WIDTH / 2) - strlen("EXCEPTION: ");

        write_string("EXCEPTION: ");
        write_string(exception_messages[regs->int_no]);

        while (1)
        {
            __asm__ __volatile__("cli; hlt");
        }
    }
    else if (regs->int_no >= 32 && regs->int_no < 48)
    {
        uint8_t irq = regs->int_no - 32;

        if (irq_routines[irq] != NULL)
        {
            irq_routines[irq](regs);
        }

        pic_send_eoi(irq);
    }
}


void pic_remap(uint8_t offset1, uint8_t offset2)
{
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW4_8086); io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW4_8086); io_wait();

    outb(PIC1_DATA, offset1); io_wait();
    outb(PIC2_DATA, offset2); io_wait();

    outb(PIC1_DATA, 0x04); io_wait();
    outb(PIC2_DATA, 0x02); io_wait();

    outb(PIC1_DATA, ICW4_8086); io_wait();
    outb(PIC2_DATA, ICW4_8086); io_wait();

    outb(PIC1_DATA, 0x00); outb(PIC2_DATA, 0x00);
}

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
    {
        outb(PIC2_COMMAND, 0x20);
    }

    outb(PIC1_COMMAND, 0x20);
}

void irq_install_handler(uint8_t irq, irq_handler_t handler)
{
    if (irq < 16)
    {
        irq_routines[irq] = handler;
    }
}

void irq_uninstall_handler(uint8_t irq)
{
    if (irq < 16)
    {
        irq_routines[irq] = NULL;
    }
}
