#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "limine.h"

#include <display.h>
#include <font.h>
#include <stmol.h>
#include <terminal.h>
#include <io.h>
#include <gdt.h>
#include <idt.h>
#include <pit.h>
#include <system.h>
#include <keyboard.h>
#include <shell.h>

void kernel_main(void)
{   
    limine_buff_init();
    init_font((void *)_binary_font_psf_start);
    display_init();
    terminal_init();
    gdt_init();
    idt_init();
    pit_init();
    keyboard_init();
    shell_init();
    
    new_line();

    write_string("Hello World this is MellOS \n\n\n");

    box_draw(120, 0x00FFB100);

    halt();
}

// testing the frambuffer will be removed later
void draw_box(int iterration, int remove, int max_size, int col_type)
{
    // Draw a 120x120 pixel white square in the center of the screen
    size_t box_size = 400; int col = 1; size_t box_col = color_array[col];
    bool shrink = false; int step_count = 0; int rem = remove; int cnt = 0;

    // uint32_t* test_buff;

    box_col = kernel_blue;
    while (iterration != 0)
    {   
        size_t start_x = (fb->width - box_size) / 2;
        size_t start_y = (fb->height - box_size) / 2;

        if (col_type == 0)
        {
            box_col = color_array[col];
        }
        
        for (size_t y = start_y; y < start_y + box_size; y++)
        {
            for (size_t x = start_x; x < start_x + box_size; x++)
            {
                draw_pixel(fb, start_x + 400, (start_y + 400) % y, box_col);
            }
        }

        // if (box_size % 800 == 0)
        // {
        //     write_string(">\n");
        // }
        

        box_size ++;

        if (box_size >= max_size)
        {   
            box_size = 0; max_size -= remove;
            if (max_size <= 0) max_size = 0, remove = rem;
            col ++;
            iterration --;
        }
        
        if (col >= (sizeof(color_array) / sizeof(size_t)))
        {
            col = 0;
        }
    }
}

// another function to test the frame buffer 
void draw_shape(int iterration, int remove, int step, int max_size, int col_type)
{
    // Draw a 120x120 pixel white square in the center of the screen
    size_t box_size = 400; int col = 1; size_t box_col = color_array[col];
    bool shrink = false; int step_count = 0; int rem = remove; int cnt = 0;

    box_col = kernel_blue;
    while (iterration != 0)
    {   
        size_t start_x = (fb->width - box_size) / 8;
        size_t start_y = (fb->height - box_size) / 2;

        if (col_type == 0)
        {
            box_col = color_array[col];
        }
        
    
        for (size_t y = start_y; y < start_y + box_size; y++)
        {
            for (size_t x = start_x; x < start_x + box_size; x++)
            {
                // fb_ptr[(y + x%y) * stride + x] = box_col;

                draw_pixel(fb, x + y, y, box_col);
            }
        }

        box_size ++;

        if (box_size >= max_size)
        {   
            box_size = 0; max_size -= remove;
            if (max_size <= 0) max_size = 0, remove = rem;
            col ++;
            iterration --;

            if (step == step_count)
            {
                remove ++;
                step_count = 0;
            }
            else
            {
                step_count ++;
            }

            if (col_type != 0)
            {
                if (cnt < 255 / 2)
                {
                    box_col -= 2;
                }
            }

            cnt ++;

            if (cnt == 256)
            {
                return;
            }
            
        }
        
        if (col >= (sizeof(color_array) / sizeof(size_t)))
        {
            col = 0;
        }

    }
}

void box_draw(int max_size, int color)
{
    size_t start_x = fb->width / 2;
    size_t start_y = fb->height / 2;

    for (size_t y = start_y; y < start_y; y++)
    {
        for (size_t x = start_x; x < start_x; x++)
        {
            draw_pixel(fb, x, y, color);
        }
    }
}