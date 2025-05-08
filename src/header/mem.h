#ifndef __MEMORY_H
#define __MEMORY_H

#include <stdint.h>
#include "bootinfo.h"
#include "def.h"


size_t get_mem_size(_EFI_MEMORY_DESCRIPTOR* mem_map, size_t mem_map_entries, size_t desc_size);
void memset(void* start, uint8_t value, size_t num);

#endif