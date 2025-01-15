#ifndef __DEF_H
#define __DEF_H

#include <stdint.h>
#include "efimem.h"

#define TRUE 1
#define FALSE 0
#define NULL (void*)0

typedef uint8_t bool;
typedef uint64_t size_t;

// Came from boot.c
typedef struct {
    uint64_t framebuffer_base;
    size_t framebuffer_size;
    uint32_t width;
    uint32_t height;
    uint32_t pixels_per_scanline;
} GRAPHICS;
typedef struct {
    EFI_MEMORY_DESCRIPTOR* map;
    size_t size;
    size_t desc_size;
} MEMORY_MAP;
typedef struct {
    GRAPHICS graphics;
    MEMORY_MAP mem_map;
} BOOT_INFO;

#endif