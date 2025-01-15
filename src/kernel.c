#include "header/kernel.h"


void setup_paging(GRAPHICS graphics, MEMORY_MAP mem_map) {
    size_t mem_map_entries = mem_map.size / mem_map.desc_size;

    read_efi_mem_map(mem_map.map, mem_map.size, mem_map.desc_size, &global_allocator);

    uint64_t kernel_size = (uint64_t)&_kernel_end - (uint64_t)&_kernel_start;
    uint64_t kernel_pages = (uint64_t)kernel_size / 4096 + 1;

    lock_pages(&global_allocator, &_kernel_start, kernel_pages);

    PAGE_TABLE* pml4 = (PAGE_TABLE*)req_page(&global_allocator);
    memset(pml4, 0, 0x1000);

    PAGE_TABLE_MANAGER page_table_mgr; page_table_mgr.pml4 = pml4;

    size_t mem_size = get_mem_size(mem_map.map, mem_map_entries, mem_map.desc_size);
    for (uint64_t i = 0; i < mem_size; i += 0x1000) {
        map_mem(&page_table_mgr, (void*)i, (void*)i);
    }

    uint64_t framebuffer_base = (uint64_t)graphics.framebuffer_base;
    size_t framebuffer_size = (uint64_t)graphics.framebuffer_size + 0x1000;
    lock_pages(&global_allocator, (void*)framebuffer_base, framebuffer_size / 0x1000 + 1);
    for (uint64_t i = framebuffer_base; i < framebuffer_base + framebuffer_size; i += 0x1000) {
        map_mem(&page_table_mgr, (void*)i, (void*)i);
    }

    asm ("movq %0, %%cr3"::"r"(pml4));
    memset((void*)graphics.framebuffer_base, 0, graphics.framebuffer_size);
}

extern void _start(void* _boot_info) {
    BOOT_INFO* boot_info = (BOOT_INFO*)_boot_info;
    GRAPHICS graphics = boot_info->graphics;
    MEMORY_MAP mem_map = boot_info->mem_map;
    assign_graphics(graphics);
    set_color(0x00aaaaaa);
    
    load_gdt(&gdtr);
    setup_paging(graphics, mem_map);
    set_idt();
    remap_pic();

    set_pos(0, 0);
    print("Kernel loaded successfully.\r\n\r\n");

    while (1);
}