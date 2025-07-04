#ifndef __OSFRT_PRINT_H
#define __OSFRT_PRINT_H

#include <stdint.h>
#include <stdarg.h>
#include "osfrt_font.h"
#include "def.h"
#include "bootinfo.h"
#include "func.h"
#include "bitmap.h"

void osfrt_printc(const char chr);
void osfrt_prints(const char* str);
void osfrt_printf(const char* str, ...);
void vosfrt_printf(const char* str, va_list args);
void set_color(uint32_t _color);
void set_pos(uint32_t _x, uint32_t _y);
void push_pos(uint32_t _x, uint32_t _y);
void pull_pos(uint32_t _x, uint32_t _y);
void assign_graphics(GRAPHICS _graphics);
void clear_screen(uint32_t color);
void clear_line(uint32_t color);

#endif