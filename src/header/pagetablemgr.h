#ifndef _PAGETABLEMGR_H
#define _PAGETABLEMGR_H

#include <stdint.h>
#include "paging.h"
#include "pageframealloc.h"
#include "mem.h"


typedef struct {
    PAGE_TABLE* pml4;
} PAGE_TABLE_MANAGER;
typedef struct {
    uint64_t pdp_index;
    uint64_t pd_index;
    uint64_t pt_index;
    uint64_t p_index;
} PAGE_MAP_INDEXER;

void map_mem(PAGE_TABLE_MANAGER* page_table_mgr, void* vir_addr, void* phys_addr);
void init_page_map_indexer(PAGE_MAP_INDEXER* page_map_indexer, uint64_t vir_addr);

#endif