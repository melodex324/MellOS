#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <idt.h>

typedef struct 
{
    char buffer[4096];
    char returned[4096];
    size_t len;
    bool ready;
} kb_line_buffer;

extern kb_line_buffer keyboard_line_buffer;

// extern bool user_press;
// extern char user_input[128];
// extern int user_input_index;

void keyboard_init();
void keyboard_handler(registers_t* regs);

#endif