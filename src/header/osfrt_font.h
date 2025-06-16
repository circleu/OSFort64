#ifndef __OSFRT_OSFRT_FONT_H
#define __OSFRT_OSFRT_FONT_H

#include <stdint.h>
#include "def.h"

#define OSFRT_FONT_WIDTH 8 // px
#define OSFRT_FONT_HEIGHT 18 // px


typedef uint8_t charbitmap[OSFRT_FONT_HEIGHT];

uint8_t* char_to_bitmap(uint8_t ch);


#endif