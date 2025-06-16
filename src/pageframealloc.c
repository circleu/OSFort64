#include "header/pageframealloc.h"


static size_t free_mem;
static size_t reserved_mem;
static size_t used_mem;
static bool init = FALSE;
BITMAP global_allocator = {0, };

void read_efi_mem_map(_EFI_MEMORY_DESCRIPTOR* mem_map, size_t mem_map_size, size_t desc_size, BITMAP* page_bitmap) {
    if (init) return;
    else init = TRUE;

    size_t mem_map_entries = mem_map_size / desc_size;

    void* largest_free_mem_seg = NULL;
    size_t largest_free_mem_seg_size = 0;

    for (size_t i = 0; i < mem_map_entries; i++) {
        _EFI_MEMORY_DESCRIPTOR* desc = (_EFI_MEMORY_DESCRIPTOR*)((uint64_t)mem_map + (i * desc_size));
        if (desc->type == 7) {
            if (desc->num_pages * 4096 > largest_free_mem_seg_size) {
                largest_free_mem_seg = (void*)desc->phys_addr;
                largest_free_mem_seg_size = desc->num_pages * 4096;
            }
        }
    }

    size_t mem_size = get_mem_size(mem_map, mem_map_entries, desc_size);
    free_mem = mem_size;
    size_t bitmap_size = mem_size / 4096 / 8 + 1;
    
    init_bitmap(page_bitmap, bitmap_size, largest_free_mem_seg);
    lock_pages(page_bitmap, (void*)page_bitmap->buffer, page_bitmap->size / 4096 + 1);
    
    for (size_t i = 0; i < mem_map_entries; i++) {
        _EFI_MEMORY_DESCRIPTOR* desc = (_EFI_MEMORY_DESCRIPTOR*)((uint64_t)mem_map + (i * desc_size));
        if (desc->type != 7) {
            reserve_pages(page_bitmap, (void*)desc->phys_addr, desc->num_pages);
        }
    }
}
void init_bitmap(BITMAP* bitmap, size_t bitmap_size, void* bitmap_addr) {
    bitmap->size = bitmap_size;
    bitmap->buffer = bitmap_addr;

    for (size_t i = 0; i < bitmap_size; i++) {
        bitmap->buffer[i] = 0;
    }
}
void* req_page(BITMAP* page_bitmap) {
    for (size_t i = 0; i < page_bitmap->size * 8; i++) {
        if (get_bitmap(page_bitmap, i) == TRUE) continue;
        lock_page(page_bitmap, (void*)(i * 4096));

        return (void*)(i * 4096);
    }

    return NULL;
}
void free_page(BITMAP* page_bitmap, void* addr) {
    uint64_t index = (uint64_t)addr / 4096;

    if (get_bitmap(page_bitmap, index) == FALSE) return;
    if (set_bitmap(page_bitmap, index, FALSE)) {
        free_mem += 4096;
        used_mem -= 4096;
    }
}
void free_pages(BITMAP* page_bitmap, void* addr, size_t page_count) {
    for (size_t i = 0; i < page_count; i++) {
        free_page(page_bitmap, (void*)((uint64_t)addr + (i * 4096)));
    }
}
void lock_page(BITMAP* page_bitmap, void* addr) {
    uint64_t index = (uint64_t)addr / 4096;

    if (get_bitmap(page_bitmap, index) == TRUE) return;
    if (set_bitmap(page_bitmap, index, TRUE)) {
        free_mem -= 4096;
        used_mem += 4096;
    }
}
void lock_pages(BITMAP* page_bitmap, void* addr, size_t page_count) {
    for (size_t i = 0; i < page_count; i++) {
        lock_page(page_bitmap, (void*)((uint64_t)addr + (i * 4096)));
    }
}
void unreserve_page(BITMAP* page_bitmap, void* addr) {
    uint64_t index = (uint64_t)addr / 4096;

    if (get_bitmap(page_bitmap, index) == FALSE) return;
    if (set_bitmap(page_bitmap, index, FALSE)) {
        free_mem += 4096;
        used_mem -= 4096;
    }
}
void unreserve_pages(BITMAP* page_bitmap, void* addr, size_t page_count) {
    for (size_t i = 0; i < page_count; i++) {
        unreserve_page(page_bitmap, (void*)((uint64_t)addr + (i * 4096)));
    }
}
void reserve_page(BITMAP* page_bitmap, void* addr) {
    uint64_t index = (uint64_t)addr / 4096;

    if (get_bitmap(page_bitmap, index) == TRUE) return;
    if (set_bitmap(page_bitmap, index, TRUE)) {
        free_mem -= 4096;
        used_mem += 4096;
    }
}
void reserve_pages(BITMAP* page_bitmap, void* addr, size_t page_count) {
    for (size_t i = 0; i < page_count; i++) {
        reserve_page(page_bitmap, (void*)((uint64_t)addr + (i * 4096)));
    }
}
size_t get_free_mem() {
    return free_mem;
}
size_t get_used_mem() {
    return used_mem;
}
size_t get_reserved_mem() {
    return reserved_mem;
}