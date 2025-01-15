#include "header/bitmap.h"


bool get_bitmap(BITMAP* bitmap, size_t index) {
    if (index > bitmap->size * 8) return FALSE;
    size_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit = 0b1 << bit_index;

    if ((bitmap->buffer[byte_index] & bit)) return TRUE;
    
    return FALSE;
}
bool set_bitmap(BITMAP* bitmap, size_t index, bool value) {
    if (index > bitmap->size * 8) return FALSE;
    size_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit = 0b1 << bit_index;

    if (value) {
        bitmap->buffer[byte_index] |= bit;
    }
    else {
        bitmap->buffer[byte_index] &= ~bit;
    }

    return TRUE;
}