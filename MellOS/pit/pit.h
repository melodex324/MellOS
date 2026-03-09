#include <stdint.h>
#include <idt.h>

#ifndef PIT_H
#define PIT_H
#define PIT_FREQUENCY 1000

extern size_t system_milli_sec;

void wait(float n);
void sleep(float ms);

void pit_init();
void on_irq0(struct InterruptRegisters* regs);

#endif