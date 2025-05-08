#ifndef _BOOTINFO_H
#define _BOOTINFO_H

#include <stdint.h>

typedef uint64_t size_t;
typedef struct {
    uint32_t            type;           // Field size is 32 bits followed by 32 bit pad
    uint32_t            pad;
    uint64_t            phys_addr;  // Field size is 64 bits
    uint64_t            vir_addr;   // Field size is 64 bits
    uint64_t            num_pages;  // Field size is 64 bits
    uint64_t            attr;      // Field size is 64 bits
} _EFI_MEMORY_DESCRIPTOR;
typedef struct {
    uint64_t framebuffer_base;
    size_t framebuffer_size;
    uint32_t width;
    uint32_t height;
    uint32_t pixels_per_scanline;
} GRAPHICS;
typedef struct {
    _EFI_MEMORY_DESCRIPTOR* map;
    size_t size;
    size_t desc_size;
} MEMORY_MAP;
typedef struct {
    GRAPHICS graphics;
    MEMORY_MAP mem_map;
    uint64_t xsdt_addr;
} BOOT_INFO;

#endif