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

size_t milisec;

char input_buffer[4096];
char empty_buffer[0];
size_t input_index = 0;
bool input_ready = false;
bool shell_loaded = false;
bool anim_activated = false;

typedef void (*command_func)(int argc, char **argv);

// typedef struct {
//     const char *name;
//     const char *description;
//     command_func function;
// } Command;

const Command commands[] = {
    { "help", "show this message", help},
    { "clear", "clear the screen", clear},
    { "uptime", "show system uptime", uptime},
    { "echo", "display a line of text", echo},
    { "buffer", "change buffer", buffer_test},
    { "pong", "play the game pong (WIP)",pong},

//  { , , },
};

const size_t NUM_COMMANDS = sizeof(commands) / sizeof(commands[0]);


void help(int argc, char **argv)
{
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (argv != NULL)
            {
                //argv[i] += *" -h";
                printf("%s", argv[i]);
                shell_process(argc, &argv[i]);
            }
        }
    }
    else 
    {
        (void)argc; (void)argv;
        for (size_t i = 0; i < NUM_COMMANDS; i++)
        {
            printf("  %s - %s\n", commands[i].name, commands[i].description);
        }
    }
    (void)argc; (void)argv;
}

void clear(int argc, char **argv)
{
    shell_initialize();

    (void)argc; (void)argv;
}

void uptime(int argc, char **argv)
{
    printf("System uptime: %d seconds\n", uptime_seconds);

    (void)argc; (void)argv;
}

void echo(int argc, char **argv)
{

    if (argc > 1)
    {

        if (strcmp(argv[1], "--help") == 0)
        {
            strlen(argv[1]);
            printf("echo will display a line of text\n");
            return;
        }

        for (int i = 1; i < argc; i++)
        {
            printf("%s", argv[i]);
            if (i < argc - 1) printf(" ");
        }
        printf("\n");
    }else
    {   
        printf("\n");
        (void)argc; (void)argv;
    }
    
}

void buffer_test(int argc, char **argv)
{
    new_line();

    if (argc > 1)
    {   
        buffer_save(shell_buffer);

        if (strcmp(argv[1], "1") == 0)
        {
            buffer_set_active(terminal_saved_buffer);
            printf("salut c'est melodex");
            wait(500);
        }

        if (strcmp(argv[1], "pong") == 0)
        {
            buffer_set_active(game_buffer);
        }

        if (strcmp(argv[1], "test") == 0)
        {
            buffer_set_active(game_buffer);
            terminal_color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);

            static_def_line_printf(11,-1,"===================================");
            static_def_line_printf(12,-1,"=TESTING BUFFER SAVING AND SWAPING=");
            static_def_line_printf(13,-1,"===================================");

            wait(3);
            buffer_save(game_buffer);
            return buffer_set_active(shell_buffer);
        }
    }else
    {
        printf("do something"); 
    }
}

void pong()
{
    buffer_save(active_buffer);
    buffer_set_active(game_buffer);

    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    static_def_line_printf(11,-1,"===================================");
    static_def_line_printf(12,-1,"===========PONG==LOADING===========");
    static_def_line_printf(13,-1,"===================================");
}