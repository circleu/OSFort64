#ifndef __PAGEFRAMEALLOC_H
#define __PAGEFRAMEALLOC_H

#include <stdint.h>
#include "def.h"
#include "efimem.h"
#include "mem.h"
#include "bitmap.h"


void read_efi_mem_map(EFI_MEMORY_DESCRIPTOR* mem_map, size_t mem_map_size, size_t desc_size, BITMAP* page_bitmap);
void init_bitmap(BITMAP* bitmap, size_t bitmap_size, void* bitmap_addr);
void* req_page(BITMAP* page_bitmap);
void free_page(BITMAP* page_bitmap, void* addr);
void free_pages(BITMAP* page_bitmap, void* addr, size_t page_count);
void lock_page(BITMAP* page_bitmap, void* addr);
void lock_pages(BITMAP* page_bitmap, void* addr, size_t page_count);
void unreserve_page(BITMAP* page_bitmap, void* addr);
void unreserve_pages(BITMAP* page_bitmap, void* addr, size_t page_count);
void reserve_page(BITMAP* page_bitmap, void* addr);
void reserve_pages(BITMAP* page_bitmap, void* addr, size_t page_count);
size_t get_free_mem();
size_t get_used_mem();
size_t get_reserved_mem();

extern BITMAP global_allocator;

#endif