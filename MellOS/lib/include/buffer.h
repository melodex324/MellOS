#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE (VGA_WIDTH * VGA_HEIGHT)

extern bool buffer_dirty;

extern uint16_t *active_buffer;
extern uint16_t terminal_saved_buffer[BUFFER_SIZE];
extern uint16_t game_buffer[BUFFER_SIZE];
extern uint16_t editor_buffer[BUFFER_SIZE];

void buffer_set_active(uint16_t *buffer);
void buffer_show(uint16_t *buffer);
void buffer_save(uint16_t *buffer);
void buffer_copy(uint16_t *dest, uint16_t *src);
void buffer_clear(uint16_t *buffer, uint8_t color);

void buffer_update();
#endif