#ifndef SHELL_H
#define SHELL_H

#include <keyboard.h>

typedef struct
{
    bool enabled;
    bool loaded;

    terminal_color shell_color;
    terminal_color cursor_color;
    terminal_color prefix_color;
    
    char cursor_shape;
} shell;

extern shell k_shell;

void shell_init();
void shell_update();

#endif