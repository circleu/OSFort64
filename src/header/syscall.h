#ifndef SYSCALL_H
#define SYSCALL_H


#include <stdint.h>
#include "print.h"

void syscall_handler();
extern __attribute__((sysv_abi)) void jmp_usr(uint64_t addr);


#endif