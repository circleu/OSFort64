#ifndef PROC_H
#define PROC_H

#include <stdint.h>
#include "pageframealloc.h"


typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t r1;
    uint64_t r2;
    uint64_t r3;
    uint64_t r4;
    uint64_t r5;
    uint64_t r6;
    uint64_t r7;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
}__attribute__((packed)) REGISTER_FRAME;
typedef struct {
    uint64_t num;
    uint64_t tmr;
    uint64_t addr;
    uint64_t pc; // program counter
    uint8_t stat;
    REGISTER_FRAME reg_frame;
}__attribute__((packed)) PROCESS_TABLE_UNIT;

void proc_cycle();

extern uint64_t* proc_table_base;


#endif