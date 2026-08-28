#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry_struct
{
    uint16_t offset1;
    uint16_t selector;
    uint8_t ist;
    uint8_t gate_type;
    uint16_t offset2;
    uint32_t offset3;
    uint32_t reserved;
}__attribute__((packed));


struct idt_pointer_struct
{
    uint16_t limit;
    uint64_t base;
}__attribute__((packed));

typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;

    uint64_t int_no;
    uint64_t error_code;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed)) registers_t;

typedef void (*irq_handler_t)(registers_t* regs);

void idt_init(void);
void idt_gate(uint8_t num, uint64_t handler, uint16_t selector, uint8_t flags, uint8_t ist);
void isr_handler(registers_t* regs);

void pic_remap(uint8_t offset1, uint8_t offset2);
void pic_send_eoi(uint8_t irq);
void irq_install_handler(uint8_t irq, irq_handler_t handler);
void irq_uninstall_handler(uint8_t irq);

#endif