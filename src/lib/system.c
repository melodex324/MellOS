#include <stdint.h>

#include <stddef.h>
#include <stdbool.h>

#include "limine.h"

#include <display.h>
#include <pit.h>

/* Sleep the processe for N milliseconds
*/
void sleep(float ms){
    uint64_t target = ticks + (uint64_t)(ms);
    while (ticks < target)
    {
        __asm__ __volatile__("hlt");
    }
}

void draw_grid()
{
    for (size_t h = 0; h < TERMINAL_HEIGHT; h++)
    {
        for (size_t y = 0; y < fb->width; y++)
        {
            draw_pixel(fb, y, (h * g_font.height), kernel_light_blue);
        }
    }
    for (size_t w = 0; w < TERMINAL_WIDTH; w++)
    {
        for (size_t x = 0; x < fb->width; x++)
        {
            draw_pixel(fb, w * g_font.width, x, kernel_light_blue);
        }
    }

}