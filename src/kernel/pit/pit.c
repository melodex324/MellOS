#include <stdint.h>
#include <stddef.h>

#include <idt.h>
#include <io.h>
#include <terminal.h>

#include <pit.h>

uint32_t frequency_hz = 1000;

volatile uint64_t ticks = 0;

void time_handler(registers_t* regs)
{
    (void)regs;
    
    ticks++;

    // write_num(48);
}

void pit_init()
{
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency_hz;

    outb(PIT_COMMAND_PORT, 0x36);

    outb(PIT_CHANNEL0_DATA, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0_DATA, (uint8_t)((divisor >> 8) & 0xFF));

    irq_install_handler(0, time_handler);
}
