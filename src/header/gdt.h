#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>
#include "def.h"


typedef struct {
    uint16_t size;
    uint64_t offs;
}__attribute__((packed)) GDTR;
typedef struct {
    uint16_t lim0 : 16;
    uint16_t base0 : 16;
    uint8_t base1 : 8;
    uint8_t a : 8;
    uint8_t lim1 : 4;
    uint8_t flag : 4;
    uint8_t base2 : 8;
}__attribute__((packed)) SEGMENT_DESCRIPTOR;
typedef struct {
    SEGMENT_DESCRIPTOR null;        // 0x00
    SEGMENT_DESCRIPTOR kernel_code; // 0x08
    SEGMENT_DESCRIPTOR kernel_data; // 0x10
    SEGMENT_DESCRIPTOR user_code;   // 0x18
    SEGMENT_DESCRIPTOR user_data;   // 0x20
}__attribute__((packed)) __attribute__((aligned(0x1000))) GDT;

__attribute__((sysv_abi)) void load_gdt(GDTR* gdtr);

extern GDT gdt;
extern GDTR gdtr;

#endif