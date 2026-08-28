#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "limine.h"

#include <stmol.h>
#include <display.h>

__attribute__((used, section(".requests")))
volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".requests")))
volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct limine_framebuffer *fb;
volatile uint32_t *fb_ptr;
size_t stride;

size_t color_array[] = 
{
    kernel_black,
    kernel_gray,
    kernel_light_gray,
    kernel_white,
    
    kernel_blue,
    kernel_cyan,
    kernel_green,
    kernel_yellow,
    kernel_orange,
    kernel_red,
    kernel_magenta,
    kernel_purple,

    kernel_light_blue,
    kernel_light_cyan,
    kernel_light_green,
    kernel_light_yellow,
    kernel_light_orange,
    kernel_light_magenta,
    kernel_light_red,
    kernel_light_purple,
};

void limine_buff_init()
{
    if (LIMINE_BASE_REVISION_SUPPORTED == false) halt();
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) halt();

    fb = framebuffer_request.response->framebuffers[0];
    fb_ptr = (volatile uint32_t *)fb->address;
    stride = fb->pitch / 4;
}

void display_init()
{
    for (size_t y = 0; y < fb->height; y++)
    {
        for (size_t x = 0; x < fb->width; x++)
        {
            fb_ptr[y * stride + x] = kernel_black;
        }
    }
}

void draw_pixel(struct limine_framebuffer *fb, uint32_t x, uint32_t y, uint32_t color) {
    if (x >= fb->width || y >= fb->height) return;
    
    uint32_t *pix = (uint32_t *)((uint8_t *)fb->address + y * fb->pitch + x * 4);
    *pix = color;
}

// rework this function later
void copy_buffer_region(struct limine_framebuffer *fb, size_t src_x, size_t src_y, size_t dest_x, size_t dest_y, size_t width, size_t height) 
{
    uint8_t *base = (uint8_t *)fb->address;
    size_t bytes_per_pixel = fb->bpp / 8;
    size_t row_bytes = width * bytes_per_pixel;

    if (dest_y > src_y)
    {
        for (size_t y = height; y > 0; y--)
        {
            uint8_t *src_line = base + ((src_y + y - 1) * fb->pitch) + (src_x * bytes_per_pixel);
            uint8_t *dest_line = base + ((dest_y + y - 1) * fb->pitch) + (dest_x * bytes_per_pixel);
            memmove(dest_line, src_line, row_bytes);
        }
    }
    else
    {
        for (size_t y = 0; y < height; y++)
        {
            uint8_t *src_line = base + ((src_y + y) * fb->pitch) + (src_x * bytes_per_pixel);
            uint8_t *dest_line = base + ((dest_y + y) * fb->pitch) + (dest_x * bytes_per_pixel);
            memmove(dest_line, src_line, row_bytes);
        }
    }
}

void clear_buffer_region(struct limine_framebuffer *fb, size_t x, size_t y, size_t width, size_t height, uint32_t color)
{
    uint8_t *base = (uint8_t *)fb->address;

    for (size_t h = 0; h < height; h++)
    {
        uint32_t *dest = (uint32_t *)(base + ((y + h) * fb->pitch)) + x;

        for (size_t w = 0; w < width; w++)
        {
            dest[w] = color;
        }
    }
}