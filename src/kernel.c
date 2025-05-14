#include "header/kernel.h"

#define USER_STACK_PAGES 4


extern void _start(void* _boot_info) {
    BOOT_INFO* boot_info = (BOOT_INFO*)_boot_info;
    GRAPHICS graphics = boot_info->graphics;
    MEMORY_MAP mem_map = boot_info->mem_map;
    XSDT* xsdt = (XSDT*)boot_info->xsdt_addr;
    assign_graphics(graphics);
    set_color(0x00aaaaaa);

// Memory Management - Setup gdt, paging, etc.
    load_gdt(&gdtr);

    size_t mem_map_entries = mem_map.size / mem_map.desc_size;

    read_efi_mem_map(mem_map.map, mem_map.size, mem_map.desc_size, &global_allocator);

    uint64_t kernel_size = (uint64_t)&_kernel_end - (uint64_t)&_kernel_start;
    uint64_t kernel_pages = (uint64_t)kernel_size / 4096 + 1;
    lock_pages(&global_allocator, &_kernel_start, kernel_pages);

    PAGE_TABLE* pml4 = (PAGE_TABLE*)req_page(&global_allocator);
    memset(pml4, 0, 0x1000);

    PAGE_TABLE_MANAGER page_table_mgr; page_table_mgr.pml4 = pml4;
    size_t mem_size = get_mem_size(mem_map.map, mem_map_entries, mem_map.desc_size);
    for (uint64_t i = 0; i < mem_size; i += 0x1000)
        map_mem(&page_table_mgr, (void*)i, (void*)i);
    
    uint64_t framebuffer_base = (uint64_t)graphics.framebuffer_base;
    size_t framebuffer_size = (uint64_t)graphics.framebuffer_size + 0x1000;
    lock_pages(&global_allocator, (void*)framebuffer_base, framebuffer_size / 0x1000 + 1);
    for (uint64_t i = framebuffer_base; i < framebuffer_base + framebuffer_size; i += 0x1000)
        map_mem(&page_table_mgr, (void*)i, (void*)i);
    
    MADT* madt = acpi_find_madt(xsdt);
    uint64_t apic_addr = (uint64_t)madt->local_apic_addr;
    map_mem(&page_table_mgr, (void*)apic_addr, (void*)apic_addr);

    uint64_t usr_space = 0x00400000;
    lock_pages(&global_allocator, (void*)usr_space, 100);
    for (uint64_t i = (uint64_t)usr_space; i < (uint64_t)usr_space + 100; i+= 0x1000)
        map_mem_usr(&page_table_mgr, (void*)i, (void*)i);
    
    uint64_t usr_stack_addr[USER_STACK_PAGES];
    for (uint64_t i = 0; i < USER_STACK_PAGES; i++) {
        usr_stack_addr[i] = (uint64_t)req_page(&global_allocator);
        lock_page(&global_allocator, (void*)usr_stack_addr[i]);
        map_mem_usr(&page_table_mgr, (void*)(0x00007fffffffe000 + i * 0x1000), (void*)usr_stack_addr[i]);
    }

    asm ("movq %0, %%cr3"::"r"(pml4));
    memset((void*)graphics.framebuffer_base, 0, graphics.framebuffer_size);

// Interrupt - idt, APIC
    set_idt();
    remap_pic();

    uint32_t* apic_base = (uint32_t*)((uint64_t)madt->local_apic_addr);
    *(apic_base + APIC_REGISTER_SPURIOUS) = 0x27; // map spurious interrupt
    *(apic_base + APIC_REGISTER_LVT_TIMER) = 0x20; // map timer interrupt
    *(apic_base + APIC_REGISTER_LVT_TIMER) = 0x21;
    *(apic_base + APIC_REGISTER_TIMER_DIV) = 0x03; // setup divide value to 16

    //////// from osdev wiki > APIC timer ////////
    // init PIC ch2 in one-shot mode
    outb(((inb(0x61) & 0xfd) | 1), 0x61);
    outb(0xb2, 0x43);
    // 1193180/100 Hz = 11931 = 0x2e9b - set frequency?
    outb(0x9b, 0x42);
    io_wait();
    outb(0x2e, 0x42);
    // reset PIT one-shot? counter (start counting)
    {
        uint8_t tmp = (inb(0x61) & 0xfe);
        outb(tmp, 0x61);
        outb(tmp | 1, 0x61);
    }
    // reset APIC timer (set counter to -1)
    *(apic_base + APIC_REGISTER_TIMER_ICR) = (uint32_t)-1;
    // wait untile PIT counter reaches zero
    while (inb(0x61) & 0x20 == 0);
    // stop APIC timer
    *(apic_base + APIC_REGISTER_LVT_TIMER) = 0x10000;
    // get current counter value
    {
        uint32_t tmp0 = *(apic_base + APIC_REGISTER_TIMER_ICR);
    /*
        make tmp0 to positive (cos it counts from -1)
        -> the value is divided by 16 so multiply by 16
        -> multiply 100 (Hz)
        -> multiply by 1000?
        -> divide by 16?

        => tmp0 -> tmp1
    */
        uint32_t tmp1 = (((uint64_t)((0xffffffff - tmp0 + 1) << 4) * 100) / 1000) >> 4;
        // sanity check? - checks if it worked well?
        if (tmp1 >= 0x10) {
            *(apic_base + APIC_REGISTER_TIMER_ICR) = tmp1;
            *(apic_base + APIC_REGISTER_LVT_TIMER) = 0x20 | 0x20000; // re-enable timer in periodic mode
        }
    }
    
//
    
    // set IA32_EFER.SCE (bit 0) to enable syscall/sysret instruction
    wrmsr(0xc0000080, rdmsr(0xc0000080) | 1);
    // write syscall handler address to IA32_LSTAR
    wrmsr(0xc0000082, (uint64_t)syscall_handler);
    // write segment selectors to IA32_STAR
    // kernel code for syscall, user code for sysret
    wrmsr(0xc0000081, (rdmsr(0xc0000081) & 0xffffffff) | ((uint64_t)0x08 << 32) | ((uint64_t)0x18 << 48));
    wrmsr(0xc0000084, rdmsr(0xc0000084) | (uint64_t)(1 << 9));

    
    set_pos(0, 0);
    print("Kernel loaded successfully.\r\n\r\n");

    while (1);

    /*
        there's a problem with entering user mode!!!
        no instruction at jumped address is the problem, I think
        so I have to implement FAT32 or make FS for this OS
        and load process manager to the jumped address
        then it will work, I guess
        I have to do that job someday so I think it's okay to do
    */

    jmp_usr(usr_space);

    while (1);
}