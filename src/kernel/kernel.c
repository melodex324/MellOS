#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "limine.h"

#include <display.h>
#include <font.h>
#include <stmol.h>
#include <terminal.h>
#include <io.h>
#include <gdt.h>
#include <idt.h>
#include <pit.h>
#include <system.h>
#include <keyboard.h>
#include <shell.h>

void kernel_main(void)
{   
    limine_buff_init();
    init_font((void *)_binary_font_psf_start);
    display_init();
    terminal_init();
    gdt_init();
    idt_init();
    pit_init();
    keyboard_init();
    shell_init();
    
    new_line();

    write_string("Hello World this is MellOS \n\n\n");

    halt();
}