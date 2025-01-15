#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>
#include "pageframealloc.h"
#include "print.h"
#include "io.h"

struct interrupt_frame;
typedef struct {
    uint16_t offs0 : 16;
    uint16_t seg : 16;
    uint8_t ist : 3;
    uint8_t zero0 : 5;
    uint8_t type : 4;
    uint8_t zero1 : 1;
    uint8_t dpl : 2;
    uint8_t p : 1;
    uint16_t offs1 : 16;
    uint32_t offs2 : 32;
    uint32_t zero2 : 32;
}__attribute__((packed)) GATE_DESCRIPTOR;
typedef struct {
    GATE_DESCRIPTOR ignore;
    GATE_DESCRIPTOR timer;
    GATE_DESCRIPTOR keyboard;
    GATE_DESCRIPTOR pf;
    GATE_DESCRIPTOR df;
    GATE_DESCRIPTOR gp;
}__attribute__((packed)) __attribute__((aligned(0x1000))) IDT;
typedef struct {
    uint16_t size;
    uint64_t offs;
}__attribute__((packed)) IDTR;

void set_idt();
void remap_pic();
static __attribute__((interrupt)) void timer(struct interrupt_frame*);
static __attribute__((interrupt)) void keyboard(struct interrupt_frame*);
static __attribute__((interrupt)) void pf(struct interrupt_frame*);
static __attribute__((interrupt)) void df(struct interrupt_frame*);
static __attribute__((interrupt)) void gp(struct interrupt_frame*);
static void panic(uint64_t exception);

extern IDT idt;
extern IDTR idtr;

#endif