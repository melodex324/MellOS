#include <stdint.h>
#include <stdbool.h>
#include <idt.h>

#ifndef SHELL_H
#define SHELL_H

extern uint16_t shell_buffer[(VGA_WIDTH * VGA_HEIGHT)];
extern uint8_t shell_color;

extern char input_buffer[4096];
extern size_t input_index;
extern bool input_ready;
extern bool shell_loaded;

void shell_init();
void shell_initialize();
void shell_process(int argc, char **argv);
int parse_input(char *input, char **argv, int max_args);
void shell_loader();
void shell_update();
void shell_handler(struct InterruptRegisters* regs);

#endif