#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

#include <display.h>
#include <font.h>
#include <stmol.h>
#include <terminal.h>
#include <keyboard.h>
#include <system.h>

#include <shell.h>
#include <command.h>

_command command[] =    // an array that will contain an the commands of the kernel (currently hardcodded but i'll try to find a way to add commands witout having to manually add them here)
{
    // { "test", "A test command which only for testing features", test}, // do not edit and run this function unless there's something to test
    { "help", "list all the commands with an explainations", help},    
    { "clear", "clear the terminal screen", clear},
    { "debug", "Will show or hide debug functions of the OS \n    For now it's not on a functional state", debug},
    // { "", "", }
};

size_t num_commands = sizeof(command) / sizeof(command[0]);

// void test(int argc, char **argv)
// {
//     printf("test func, does nothing");
// }

void help(int argc, char **argv)
{
    current_offset.width = 4;

    if (argc > 1)
    {
        char *buff[2]; buff[1] = "help";
        for (size_t i = 0; i < num_commands; i++)
        {
            if (strcmp(argv[1], command[i].name) == 0)
            {
                command[i].function(2, buff);
                // printf("    %s - %s\n", command[i].name, command[i].description);
            }
        }
    }
    else
    {        
        for (size_t i = 0; i < num_commands; i++)
        {
            printf("%s - %s\n", command[i].name, command[i].description);
        }
        current_offset.width = 0;
    }
}

void clear(int argc, char **argv)
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "help") == 0)
        {
            printf("To clear the current terminal buffer");
            return;
        }
    }

    terminal_clear();
}

void debug(int argc, char **argv)
{
    _subcommand debug_subcom[] =
    {
        { "grid", draw_grid},
        { "help", help},
    };

    if (argc > 1)
    {
        return;
    }


    for (size_t i = 0; i < sizeof(debug_subcom)/sizeof(debug_subcom[0]); i++)
    {
        if (strcmp(argv[1], debug_subcom[i].name) == 0)   // <- the ONE comparison
        {
            debug_subcom[i].function(argc - 1, argv + 1);
            return;
        }
    }

    printf("azerty");
    
}