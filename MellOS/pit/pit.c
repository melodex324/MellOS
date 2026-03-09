#include <stdint.h>
#include <stmol.h>
#include <vga.h>
#include <util.h>
#include <idt.h>

#include <pit.h>

#include <kernel_util.h>
#include <shell.h>
#include <buffer.h>


uint32_t ticks;
const uint32_t frequency = 1000;
size_t system_milli_sec = 0;
int fps = 0;
int mfps;

void on_irq0(struct InterruptRegisters *regs)
{   
    (void)regs;
    update_cursor(terminal_column, terminal_row);
    ticks += 1;
    
    if (ticks % 16 == 0)
    {
        fps += 1;
        clear_row(23);
        static_def_line_printf(23,-1,"Buffer update : %d", mfps);
        buffer_update();
    }

    if (ticks % 1 == 0)
    {
        system_milli_sec += 1;
        kernel_util_handler(regs);
        shell_handler(regs);
        if (uptime_enable == true && system_milli_sec % frequency/2 == 0)
        {
            uptime_bar();
        }

        if (system_milli_sec % frequency/2 == 0)
        {
            mfps = fps;
            fps = 0;
        }
    }
    
}

void wait(float s){
    uint64_t target = ticks + (uint64_t)(s * frequency);
    while (ticks < target)
    {
        asm volatile("hlt");
    }
}

void sleep(float ms){
    uint64_t target = ticks + (uint64_t)(ms);
    while (ticks < target)
    {
        asm volatile("hlt");
    }
}

void pit_init()
{
    ticks = 0;
    irq_install_handler(0, &on_irq0);

    uint32_t divisor = 1193180 / frequency;

    outPortB(0x43, 0x36);
    outPortB(0x40, (uint8_t)(divisor & 0xFF));
    outPortB(0x40, (uint8_t)((divisor >> 8) & 0xFF));

    terminal_writestring("PIT initiated ");
}