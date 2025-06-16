#include "header/syscall.h"


void syscall_handler() {
    //       rax  rdi   rsi   rdx   r10   r8    r9
    uint64_t num, arg1, arg2, arg3, arg4, arg5, arg6;
    asm volatile (
        "movq %%rax, %0;"
        "movq %%rdi, %1;"
        "movq %%rsi, %2;"
        "movq %%rdx, %3;"
        "movq %%r10, %4;"
        "movq %%r8, %5;"
        "movq %%r9, %6;"
        :"=r"(num), "=r"(arg1), "=r"(arg2), "=r"(arg3), "=r"(arg4), "=r"(arg5), "=r"(arg6)
    );

    asm volatile (
        "pushq %rcx;"
        "pushq %r11;"
    );

    switch (num) {
        case 0: { //null; not using

            break;
        }
        case 1: {
            break;
        }
        case 12: {
            prints("\r\n\r\n yay \r\n\r\n");
            while(1);
            break;
        }
    }

    asm volatile (
        "popq %r11;"
        "popq %rcx;"
        "sysretq;"
    );
}