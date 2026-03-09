#include <stdint.h>
#include <stmol.h>
#include <util.h>
#include <pit.h>
#include <keyboard.h>
#include <buffer.h>

#include <kernel_util.h>

//TODO make a shortcut to disable it

#define uptime_row 24

bool uptime_enable;
size_t uptime_margin = 0;

size_t uptime_seconds;
size_t col_num;

void uptime_init()
{
    system_milli_sec = 0; uptime_seconds = 0;
    uptime_enabled(true);
    uptime_bar();
}

void uptime_enabled(bool uptime_enabled)
{
    if (uptime_enabled == true)
    {
        clear_row(uptime_row);
        uptime_enable = true;
        // margins += 1;
    }
    else if (uptime_enabled == false)
    {
        uptime_enable = false;
        clear_row(uptime_row);
        margins -= 1;
    }
}

void uptime_bar()
{	
    if (col_num > 7)
    {
        col_num = 0;
    }else if (system_milli_sec ++)
    {
        col_num ++;
    }

    uint8_t print_temp = terminal_color;

    terminal_color = vga_entry_color(col_num, VGA_COLOR_WHITE);
    clear_row(uptime_row);
    static_def_line_printf(uptime_row, -1,"DEBUG MENU : System ON for %ds | X %d : Y %d", uptime_seconds, terminal_column, terminal_row);

    terminal_color = print_temp;
}

void kernel_util_handler(struct InterruptRegisters* regs)
{
    (void)regs;

	uptime_seconds = system_milli_sec / 1000;
}

void kernel_util_init()
{
    terminal_writestring("kernel_util initiated ");
}