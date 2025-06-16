#ifndef __PRINT_H
#define __PRINT_H

#include <stdint.h>
#include <stdarg.h>
#include "osfrt_font.h"
#include "def.h"
#include "bootinfo.h"
#include "func.h"
#include "bitmap.h"

void printc(const char chr);
void prints(const char* str);
void printf(const char* str, ...);
void set_color(uint32_t _color);
void set_pos(uint32_t _x, uint32_t _y);
void push_pos(uint32_t _x, uint32_t _y);
void pull_pos(uint32_t _x, uint32_t _y);
void assign_graphics(GRAPHICS _graphics);
void clear_screen(uint32_t color);
void clear_line(uint32_t color);

#endif