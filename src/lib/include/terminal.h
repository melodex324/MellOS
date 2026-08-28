#ifndef TERMINAL_H
#define TERMINAL_H

#include <display.h>

extern size_t terminal_row;
extern size_t terminal_column;
extern uint32_t terminal_color[2];

typedef struct 
{
    uint32_t fg;
    uint32_t bg;
} terminal_col;

extern terminal_col current_theme;
extern terminal_col default_theme;

void terminal_set_color(enum kernel_color fg, enum kernel_color bg);
void terminal_set_theme(terminal_col theme);

void make_lines(size_t lines);
void new_line(void);

void clear_row(size_t row);

void write_char(char c);
void write_string(const char* string);

void terminal_scroll(void);

#endif