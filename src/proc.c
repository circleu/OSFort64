#include "header/proc.h"


uint64_t* proc_table_base;
uint64_t proc_num_counter;

void proc_cycle() {
    __asm__ volatile (
        "movq $12, %rax;"
        "syscall;"
    );

    while (1);
}