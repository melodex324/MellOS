#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <vga.h>
#include <util.h>

#ifndef STMOL_H
#define STMOL_H

extern size_t margins;

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

extern uint16_t *saved_buffer;

extern size_t terminal_row;
extern size_t terminal_column;
extern uint8_t terminal_color;
// extern uint16_t *terminal_buffer;


//memset is declared in util.h
void *memcpy(void *dest, const void* src, size_t n);
int memcmp(const void *ptr1, const void *ptr2, size_t n);
void *memmove(void *dest, const void *src, size_t count);

void append(char s[], char n);

extern size_t strlen(const char* str);
char *strcpy(char *dest, const char *src);
int strcmp(const char *str1, const char *str2);
char *strcat(char *dest, const char *src);

void swap(char *a, char *b);
void reverse(char *str, size_t len);
char *itoa(int value, char *str, int base);

void make_line(int lines);
void new_line();

void vprintf(const char *format, va_list args);
void printf(const char *format, ...);               //normal C print
void print();                                       //python like print

void update_cursor(int x, int y);

void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_scroll();

void copy_row(size_t src, size_t dest);
void clear_row(size_t row);
void copy_current_row(size_t dest);
void clear_current_row();

//Test & experimental function needs works
void static_def_line_printf(int row, int col, const char *format, ...);
void static_set_line_printf(int row, const char *format, ...);
void static_line_printf(const char *format, ...);
#endif