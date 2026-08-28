#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include <stddef.h>

#include <limine.h>

extern uint8_t _binary_font_psf_start[];
extern uint8_t _binary_font_psf_end[];

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04
#define PSF2_MAGIC  0x864ab572

typedef struct {
    uint8_t magic[2];     // 0x36, 0x04
    uint8_t mode;         // Font mode
    uint8_t charsize;     // Glyph height in bytes (width is always 8)
} __attribute__((packed)) psf1_header_t;

typedef struct {
    uint32_t magic;       // 0x864ab572
    uint32_t version;     // Usually 0
    uint32_t headersize;  // Offset where glyph data starts
    uint32_t flags;       // Flags (e.g. HAS_UNICODE_TABLE)
    uint32_t length;      // Number of glyphs
    uint32_t charsize;    // Byte size per glyph
    uint32_t height;      // Height in pixels
    uint32_t width;       // Width in pixels
} __attribute__((packed)) psf2_header_t;

typedef struct {
    uint8_t *glyph_buffer;
    uint32_t width;
    uint32_t height;
    uint32_t charsize;
} font_t;

extern font_t g_font;

void init_font(void *font_raw_data);
void draw_font(struct limine_framebuffer *fb, char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg);

#endif