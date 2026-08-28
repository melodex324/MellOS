#include <stdint.h>
#include <stddef.h>

#include <font.h>
#include <limine.h>
#include <stmol.h>
#include <display.h>

font_t g_font;

void init_font(void *font_raw_data) {
    psf1_header_t *psf1 = (psf1_header_t *)font_raw_data;
    psf2_header_t *psf2 = (psf2_header_t *)font_raw_data;

    if (psf1->magic[0] == PSF1_MAGIC0 && psf1->magic[1] == PSF1_MAGIC1) {
        g_font.width = 8;
        g_font.height = psf1->charsize;
        g_font.charsize = psf1->charsize;
        g_font.glyph_buffer = (uint8_t *)font_raw_data + sizeof(psf1_header_t);
    } else if (psf2->magic == PSF2_MAGIC) {
        g_font.width = psf2->width;
        g_font.height = psf2->height;
        g_font.charsize = psf2->charsize;
        g_font.glyph_buffer = (uint8_t *)font_raw_data + psf2->headersize;
    } else {
        // Invalid or unsupported font header
    }
}

void draw_font(struct limine_framebuffer *fb, char c, uint32_t x, uint32_t y, uint32_t* fg, uint32_t* bg) {
    uint8_t *glyph = g_font.glyph_buffer + ((unsigned char)c * g_font.charsize);
    uint32_t bytes_per_line = (g_font.width + 7) / 8;

    for (uint32_t cy = 0; cy < g_font.height; cy++) {
        for (uint32_t cx = 0; cx < g_font.width; cx++) {
            uint32_t byte_offset = cy * bytes_per_line + (cx / 8);
            uint8_t bit_mask = 0x80 >> (cx % 8);

            if (glyph[byte_offset] & bit_mask) {
                draw_pixel(fb, x + cx, y + cy, fg);
            } else if (bg != 0xFFFFFFFF) { // Optional background color
                draw_pixel(fb, x + cx, y + cy, bg);
            }
        }
    }
}