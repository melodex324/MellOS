#include <stdint.h>
#include <stmol.h>
#include <buffer.h>

#define BUFFER_SIZE (VGA_WIDTH * VGA_HEIGHT)

bool buffer_dirty = false;

uint16_t *active_buffer = (uint16_t*)VGA_MEMORY;

uint16_t terminal_saved_buffer[BUFFER_SIZE];
uint16_t game_buffer[BUFFER_SIZE];
uint16_t editor_buffer[BUFFER_SIZE];


void buffer_set_active(uint16_t *buffer)
{
    active_buffer = buffer;
    terminal_column = buffer[-1];
    terminal_row = buffer[-2];
}

void buffer_show(uint16_t *buffer)
{
    if (buffer == NULL)
    {
        return;
    }
    memcpy(active_buffer, buffer, BUFFER_SIZE * sizeof(uint16_t));
    buffer_set_active(buffer);
}

void buffer_save(uint16_t *buffer)
{
    if (buffer == NULL) 
    {
        return;
    }
    memcpy(buffer, active_buffer, BUFFER_SIZE * sizeof(uint16_t));
    buffer[-1] = terminal_column;
    buffer[-2] = terminal_row;
}

void buffer_copy(uint16_t *dest, uint16_t *src)
{
    if (dest == NULL || src == NULL) 
    {
        return;
    }
    memcpy(dest, src, BUFFER_SIZE * sizeof(uint16_t));
}

void buffer_clear(uint16_t *buffer, uint8_t color)
{
    if (buffer == NULL)
    {
        return;
    }
    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = vga_entry(' ', color);
    }
}

void buffer_update()
{
    if (active_buffer == NULL || !buffer_dirty) return;
    memcpy((uint16_t*)VGA_MEMORY, active_buffer, BUFFER_SIZE * sizeof(uint16_t));
    buffer_dirty = false;
    // static_set_line_printf(21,"%d ; %d", active_buffer[- 1], active_buffer[-2]);
}