#ifndef PIT_H
#define PIT_H

#define PIT_CHANNEL0_DATA 0x40
#define PIT_COMMAND_PORT  0x43
#define PIT_BASE_FREQUENCY 1193182

extern volatile uint64_t ticks;

void pit_init();

#endif