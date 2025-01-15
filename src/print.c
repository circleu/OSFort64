#include "header/print.h"


static GRAPHICS graphics;
static uint32_t color = 0x00aaaaaa;
static uint32_t x = 0;
static uint32_t y = 0;

void print(const char* str) {
    for (size_t c = 0; str[c] != 0; c++) {
        switch (str[c]) {
            case '\n': y++; continue;
            case '\r': x = 0; continue;
        }

        uint8_t* ch = char_to_bitmap(str[c]);
        if (ch == NULL) continue;

        for (uint8_t i = 0; i < FONT_HEIGHT; i++) {
            for (uint8_t j = 0; j < FONT_WIDTH; j++) {
                if (*(ch + (i * FONT_WIDTH) + j)) *(uint32_t*)(graphics.framebuffer_base + (graphics.pixels_per_scanline * (y * FONT_HEIGHT + i) * 4) + ((x * FONT_WIDTH + j) * 4)) = color;
            }
        }

        x++;
        if (x > graphics.pixels_per_scanline / FONT_WIDTH) {
            x -= graphics.pixels_per_scanline / FONT_WIDTH;
            y++;
        }
    }
}
void set_color(uint32_t _color) {
    color = _color;
}
void set_pos(uint32_t _x, uint32_t _y) {
    x = _x;
    y = _y;
}
void push_pos(uint32_t _x, uint32_t _y) {
    x += _x;
    y += _y;
}
void pull_pos(uint32_t _x, uint32_t _y) {
    x -= _x;
    y -= _y;
}
void assign_graphics(GRAPHICS _graphics) {
    graphics = _graphics;
}
void clear_screen(uint32_t color) {
    for (uint32_t i = 0; i < graphics.height; i++) {
        for (uint32_t j = 0; j < graphics.width; j++) {
            *(uint32_t*)(graphics.framebuffer_base + (graphics.pixels_per_scanline * i * 4) + (j * 4)) = color;
        }
    }
}