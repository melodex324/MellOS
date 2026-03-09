#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <vga.h>
#include <stmol.h>
#include <util.h>
#include <idt.h>
#include <pit.h>
#include <kernel_util.h>
#include <buffer.h>

extern size_t milisec;

extern char input_buffer[4096];
extern char empty_buffer[0];
extern size_t input_index;
extern bool input_ready;
extern bool shell_loaded;
extern bool anim_activated;

typedef void (*command_func)(int argc, char **argv);

typedef struct {
    const char *name;
    const char *description;
    command_func function;
} Command;

void help(int argc, char **argv);
void clear(int argc, char **argv);
void uptime(int argc, char **argv);
void echo(int argc, char **argv);
void buffer_test();
void pong();

extern const Command commands[];
extern const size_t NUM_COMMANDS;

#endif