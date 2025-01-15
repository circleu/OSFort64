#include "header/mem.h"


size_t get_mem_size(EFI_MEMORY_DESCRIPTOR* mem_map, size_t mem_map_entries, size_t desc_size) {
    static size_t mem_size = 0;

    for (size_t i = 0; i < mem_map_entries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mem_map + (i * desc_size));
        mem_size += desc->num_pages * 4096;
    }

    return mem_size;
}

void memset(void* start, uint8_t value, size_t num) {
    for (size_t i = 0; i < num; i++) {
        *(uint8_t*)((uint64_t)start + i) = value;
    }
}