#ifndef TERMINAL_H
#define TERMINAL_H

#include <display.h>

extern size_t terminal_row;
extern size_t terminal_column;

typedef struct 
{
    uint32_t fg;
    uint32_t bg;
} terminal_color;

typedef struct 
{
    int height;
    int width;
} terminal_offset;

extern terminal_color current_theme;
extern terminal_color default_theme;

extern terminal_offset current_offset;
extern terminal_offset default_offset;

void terminal_init();

void terminal_set_color(enum kernel_color fg, enum kernel_color bg);
void terminal_set_theme(terminal_color theme);

void make_lines(size_t lines);
void new_line(void);

void clear_row(size_t row);

void write_char(char c);
void write_string(const char* string);

void terminal_scroll(void);

void terminal_clear();

#endif