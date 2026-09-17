#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

#include <display.h>
#include <font.h>
#include <stmol.h>
#include <terminal.h>
#include <keyboard.h>

#include <shell.h>
#include <command.h>

shell k_shell;

#define SHELL_CURSOR terminal_set_theme(k_shell.cursor_color); printf("%d%c ", *lines + 1, k_shell.cursor_shape); terminal_set_theme(k_shell.shell_color);

kb_line_buffer *shellkeyboard;
size_t *lines = &terminal_row;


void shell_init()
{
    printf("Shell Initiated");

    k_shell.shell_color.bg = -kernel_black; k_shell.shell_color.fg = kernel_light_gray;
    k_shell.cursor_color.bg = k_shell.shell_color.bg; k_shell.cursor_color.fg = kernel_light_blue;
    k_shell.prefix_color.bg = k_shell.shell_color.bg; k_shell.prefix_color.fg = kernel_light_green;

    k_shell.cursor_shape = '>';

    k_shell.enabled = true;
}

void shell_loader()
{
    if (k_shell.loaded == false)
    {        
        k_shell.loaded = true;
        terminal_set_theme(k_shell.prefix_color);
        printf("Shell Loaded|");
        SHELL_CURSOR
    }

    return;
}

void shell_process(int argc, char **argv) 
{
    for (size_t i = 0; i < num_commands; i++)
    {
        if (strcmp(argv[0], command[i].name) == 0)
        {
            command[i].function(argc, argv);
            return;
        }   
    }
    
    printf("%s | Command not found \n", argv[0]);
}

void shell_parser(char *buffer, char **argv)
{
    if (buffer == NULL || buffer[0] == '\0')
    {
        return;
    }
    
    int argc = 0;

    for (size_t i = 0; buffer[i] != '\0'; i++)
    {
        while (buffer[i] == ' ')
        {
            i++;
        }

        if (buffer[i] == '\0')
        {
            break;
        }
        
        argv[argc++] = &buffer[i];

        while (buffer[i] != ' ' && buffer[i] != '\0')
        {
            i++;
        }
        
        if (buffer[i] == ' ')
        {
            buffer[i] = '\0';
        }
    }

    if (argc > 0)
    {
        shell_process(argc, argv);
    }
}

void shell_update()
{
    if (k_shell.enabled == false)
    {
        return;
    }

    if (k_shell.loaded == false && k_shell.enabled == true)
    {
        shell_loader();
    }

    terminal_set_theme(k_shell.shell_color);

    if (keyboard_line_buffer.ready == true)
    {
        char *argv[64];
        shell_parser(keyboard_line_buffer.returned, argv);
        
        memset(keyboard_line_buffer.returned, '\0', sizeof(keyboard_line_buffer.returned));
        keyboard_line_buffer.ready  = false;
        SHELL_CURSOR
    }
}