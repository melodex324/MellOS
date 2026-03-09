#ifndef KERNEL_UTIL_H
#define KERNEL_UTIL_H

extern bool uptime_enable;
extern size_t uptime_seconds;

void uptime_init();
void uptime_enabled(bool uptime_enabled);
void uptime_bar();

void kernel_util_init();
void kernel_util_handler(struct InterruptRegisters* regs);

#endif