#include "header/pagetablemgr.h"


void map_mem(PAGE_TABLE_MANAGER* page_table_mgr, void* vir_addr, void* phys_addr) {
    PAGE_MAP_INDEXER indexer; init_page_map_indexer(&indexer, (uint64_t)vir_addr);
    PAGE_TABLE_ENTRY pe;

    pe = page_table_mgr->pml4->entries[indexer.pdp_index];
    PAGE_TABLE* pdp;
    if (!pe.p) {
        pdp = (PAGE_TABLE*)req_page(&global_allocator);
        memset(pdp, 0, 0x1000);
        pe.addr = (uint64_t)pdp >> 12;
        pe.p = TRUE;
        pe.rw = TRUE;
        page_table_mgr->pml4->entries[indexer.pdp_index] = pe;
    }
    else {
        pdp = (PAGE_TABLE*)((uint64_t)pe.addr << 12);
    }

    pe = pdp->entries[indexer.pd_index];
    PAGE_TABLE* pd;
    if (!pe.p) {
        pd = (PAGE_TABLE*)req_page(&global_allocator);
        memset(pd, 0, 0x1000);
        pe.addr = (uint64_t)pd >> 12;
        pe.p = TRUE;
        pe.rw = TRUE;
        pdp->entries[indexer.pd_index] = pe;
    }
    else {
        pd = (PAGE_TABLE*)((uint64_t)pe.addr << 12);
    }

    pe = pd->entries[indexer.pt_index];
    PAGE_TABLE* pt;
    if (!pe.p) {
        pt = (PAGE_TABLE*)req_page(&global_allocator);
        memset(pt, 0, 0x1000);
        pe.addr = (uint64_t)pt >> 12;
        pe.p = TRUE;
        pe.rw = TRUE;
        pd->entries[indexer.pt_index] = pe;
    }
    else {
        pt = (PAGE_TABLE*)((uint64_t)pe.addr << 12);
    }
    
    pe = pt->entries[indexer.p_index];
    pe.addr = (uint64_t)phys_addr >> 12;
    pe.p = TRUE;
    pe.rw = TRUE;
    pt->entries[indexer.p_index] = pe;
}
void map_mem_usr(PAGE_TABLE_MANAGER* page_table_mgr, void* vir_addr, void* phys_addr) {
    PAGE_MAP_INDEXER indexer; init_page_map_indexer(&indexer, (uint64_t)vir_addr);
    PAGE_TABLE_ENTRY pe;

    pe = page_table_mgr->pml4->entries[indexer.pdp_index];
    PAGE_TABLE* pdp;
    if (!pe.p) {
        pdp = (PAGE_TABLE*)req_page(&global_allocator);
        memset(pdp, 0, 0x1000);
        pe.addr = (uint64_t)pdp >> 12;
        pe.p = TRUE;
        pe.rw = TRUE;
        pe.us = TRUE;
        pe.xd = FALSE;
        page_table_mgr->pml4->entries[indexer.pdp_index] = pe;
    }
    else {
        pdp = (PAGE_TABLE*)((uint64_t)pe.addr << 12);
    }

    pe = pdp->entries[indexer.pd_index];
    PAGE_TABLE* pd;
    if (!pe.p) {
        pd = (PAGE_TABLE*)req_page(&global_allocator);
        memset(pd, 0, 0x1000);
        pe.addr = (uint64_t)pd >> 12;
        pe.p = TRUE;
        pe.rw = TRUE;
        pe.us = TRUE;
        pe.xd = FALSE;
        pdp->entries[indexer.pd_index] = pe;
    }
    else {
        pd = (PAGE_TABLE*)((uint64_t)pe.addr << 12);
    }

    pe = pd->entries[indexer.pt_index];
    PAGE_TABLE* pt;
    if (!pe.p) {
        pt = (PAGE_TABLE*)req_page(&global_allocator);
        memset(pt, 0, 0x1000);
        pe.addr = (uint64_t)pt >> 12;
        pe.p = TRUE;
        pe.rw = TRUE;
        pe.us = TRUE;
        pe.xd = FALSE;
        pd->entries[indexer.pt_index] = pe;
    }
    else {
        pt = (PAGE_TABLE*)((uint64_t)pe.addr << 12);
    }
    
    pe = pt->entries[indexer.p_index];
    pe.addr = (uint64_t)phys_addr >> 12;
    pe.p = TRUE;
    pe.rw = TRUE;
    pe.us = TRUE;
    pe.xd = FALSE;
    pt->entries[indexer.p_index] = pe;
}
void init_page_map_indexer(PAGE_MAP_INDEXER* page_map_indexer, uint64_t vir_addr) {
    vir_addr >>= 12;
    page_map_indexer->p_index = vir_addr & 0x1ff;
    vir_addr >>= 9;
    page_map_indexer->pt_index = vir_addr & 0x1ff;
    vir_addr >>= 9;
    page_map_indexer->pd_index = vir_addr & 0x1ff;
    vir_addr >>= 9;
    page_map_indexer->pdp_index = vir_addr & 0x1ff;
}