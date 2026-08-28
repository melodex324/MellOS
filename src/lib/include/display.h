#ifndef DISPLAY_H
#define DISPLAY_H

#include "limine.h"
#include <font.h>

#define TEXT_WIDTH g_font.width
#define TEXT_HEIGHT g_font.height

__attribute__((used, section(".requests")))
extern volatile struct limine_framebuffer_request framebuffer_request;

extern struct limine_framebuffer *fb;
extern volatile uint32_t *fb_ptr;
extern size_t stride;

#define TERMINAL_WIDTH (fb->width / TEXT_WIDTH)
#define TERMINAL_HEIGHT (fb->height / TEXT_HEIGHT)

enum kernel_color
{
    kernel_black = 0x00000000,
    kernel_gray = 0x00606060,
    kernel_light_gray = 0x00C0C0C0,
    kernel_white = 0x00FFFFFF,

    kernel_blue = 0x000000FF,
    kernel_cyan = 0x0000FFFF,
    kernel_green = 0x0000FF00,
    kernel_yellow = 0x00FFFF00,
    kernel_orange = 0x00FF8000,
    kernel_magenta = 0x00FF00FF,
    kernel_red = 0x00FF0000,
    kernel_purple = 0x00A000FF,

    kernel_light_blue = 0x004080FF,
    kernel_light_cyan = 0x0080FFFF,
    kernel_light_green = 0x0080FF40,
    kernel_light_yellow = 0x00FFFF80,
    kernel_light_orange = 0x00FFA040,
    kernel_light_red = 0x00FF4040,
    kernel_light_magenta = 0x00FF60FF,
    kernel_light_purple = 0x00C028FF,
};

extern size_t color_array[20];

extern void limine_buff_init();
extern void display_init();

void draw_pixel(struct limine_framebuffer *fb, uint32_t x, uint32_t y, uint32_t color);

void copy_buffer_region(struct limine_framebuffer *fb, size_t src_x, size_t src_y, size_t dest_x, size_t dest_y, size_t width, size_t height);
void clear_buffer_region(struct limine_framebuffer *fb, size_t x, size_t y, size_t width, size_t height, uint32_t color);


#endif