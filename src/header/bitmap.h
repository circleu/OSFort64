#ifndef __BITMAP_H
#define __BITMAP_H

#include <stdint.h>
#include "def.h"


typedef struct {
    uint8_t* buffer;
    size_t size;
} BITMAP;

bool get_bitmap(BITMAP* bitmap, size_t index);
bool set_bitmap(BITMAP* bitmap, size_t index, bool value);

#endif