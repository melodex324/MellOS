#include <stdint.h>
#include <vga.h>
#include <stmol.h>
#include <util.h>
#include <idt.h>
#include <pit.h>
#include <kernel_util.h>
#include <buffer.h>

#include <shell.h>
#include <commands.h>

#define shell_cursor terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK); printf("> "); terminal_color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);


uint16_t shell_buffer[(VGA_WIDTH * VGA_HEIGHT)];
uint8_t shell_color;

void shell_init()
{
    printf("Shell initiated ");
}

void shell_initialize()
{
    terminal_row = 0;
	terminal_column = 0;

    buffer_clear(shell_buffer, terminal_color);
}

void shell_process(int argc, char **argv)
{
    for (size_t i = 0; i < NUM_COMMANDS; i++)
    {
        if (strcmp(argv[0], commands[i].name) == 0)
        {
            commands[i].function(argc, argv);
            return;
        }
    }
    printf("Unknown command: %s\n", argv[0]);
}

int parse_input(char *input, char **argv, int max_args)
{
    if (input == NULL || input[0] == '\0')
        return 0;

    int argc = 0;

    for (size_t i = 0; input[i] != '\0'; i++)
    {
        while (input[i] == ' ')
            i++;

        if (input[i] == '\0')
            break;

        argv[argc++] = &input[i];

        if (argc >= max_args)
            break;

        while (input[i] != ' ' && input[i] != '\0')
            i++;

        if (input[i] == ' ')
            input[i] = '\0';
    }

    if (argc > 0)
        shell_process(argc, argv);

    return argc;
}

void shell_loader()
{
    anim_activated = true;
    if (shell_loaded == false)
    {
        shell_initialize();
	    shell_color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
        buffer_show(shell_buffer);

        shell_loaded = true;
        shell_cursor
    }
}

void shell_update()
{
    if (input_ready)
    {
        input_buffer[input_index] = '\0';
        new_line();

        char *argv[16];
        parse_input(input_buffer, argv, 16);

        memset(input_buffer, 0, sizeof(input_buffer));
        input_index = 0;
        input_ready = false;
        shell_cursor
    }
}

int frame = 0;
static void shell_anim(bool update)
{
    char anim[][17] = {
        ""
        "S",
        "SH",
        "SHE",
        "SHEL",
        "SHELL",
        "SHELL",
        "SHELL",
        "SHELL",
        "SHELL",
        "SHELL",
        "SHELL",
        "SHEL",
        "SHE",
        "SH",
        "S",
    };
    if (update == true && anim_activated == true)
    { 
        clear_row(22);
        static_set_line_printf(22,"%s", anim[frame]); 
        update = false;
        frame ++;
    }
    
    if (frame == (sizeof(anim) / 16)+1)
    {
        frame = 0;
    }
}

void shell_handler(struct InterruptRegisters* regs)
{
    (void)regs;
    milisec = system_milli_sec;
    if (milisec % 250 == 0)
    {  
        milisec = 0;
        shell_anim(true);
    }
    
}
