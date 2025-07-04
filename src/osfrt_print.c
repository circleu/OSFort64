#include "header/osfrt_print.h"

static GRAPHICS graphics;
static uint32_t color = 0x00aaaaaa;
static uint32_t x = 0;
static uint32_t y = 0;

void osfrt_printc(const char chr) {
    switch (chr) {
        case '\n': y++; goto END;
        case '\r': x = 0; goto END;
    }

    BITMAP ch;
    ch.buffer = char_to_bitmap(chr);
    ch.size = OSFRT_FONT_HEIGHT;
    if (ch.buffer == NULL) goto END;

    for (uint8_t i = 0; i < OSFRT_FONT_HEIGHT; i++) {
        for (uint8_t j = 0; j < OSFRT_FONT_WIDTH; j++) {
            if (get_bitmap(&ch, i * OSFRT_FONT_WIDTH + (OSFRT_FONT_WIDTH - 1 - j))) *(uint32_t*)(graphics.framebuffer_base + (graphics.pixels_per_scanline * (y * OSFRT_FONT_HEIGHT + i) * 4) + ((x * OSFRT_FONT_WIDTH + j) * 4)) = color;
        }
    }

    x++;
    if (x >= graphics.pixels_per_scanline / OSFRT_FONT_WIDTH) {
        x -= graphics.pixels_per_scanline / OSFRT_FONT_WIDTH;
        y++;
    }
    END:
}
void osfrt_prints(const char* str) {
    for (size_t c = 0; str[c] != 0; c++) osfrt_printc(str[c]);
}
void osfrt_printf(const char* str, ...) {
    va_list args;
    va_start(args, str);
    vosfrt_printf(str, args);
    va_end(args);
}
void vosfrt_printf(const char* str, va_list args) {
    for (size_t c = 0; str[c] != 0; c++) {
        switch (str[c]) {
            case '%': {
                c++;
                switch (str[c]) {
                    case 'd': {
                        osfrt_prints(itoa(va_arg(args, uint64_t)));
                        continue;
                    }
                    case 's': {
                        osfrt_prints(va_arg(args, uint8_t*));
                        continue;
                    }
                    default: {
                        c--;
                        break;
                    }
                }
                break;
            }
        }
        osfrt_printc(str[c]);
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
void clear_line(uint32_t color) {
    for (uint32_t i = 0; i < graphics.width; i++) {
        for (uint8_t j = 0; j < OSFRT_FONT_HEIGHT; j++) {
            *(uint32_t*)(graphics.framebuffer_base + (graphics.pixels_per_scanline * j * 4) + (i * 4)) = color;
        }
    }
}