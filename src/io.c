#include "header/io.h"


void outb(uint8_t origin, uint16_t dest) {
    asm volatile ("movb %0, %%al; movw %1, %%dx; outb %%al, %%dx"::"r"(origin), "r"(dest));
}
uint8_t inb(uint16_t origin) {
    uint8_t ret;
    asm volatile ("movw %1, %%dx; inb %%dx, %%al; movb %%al, %0":"=r"(ret):"r"(origin));
    return ret;
}
void io_wait() {
    asm volatile ("movb $0x00, %al; movw $0x0080, %dx; outb %al, %dx");
}
void rdmsr(uint32_t msr, uint32_t* l, uint32_t* h) {
    asm volatile ("rdmsr":"=a"(*l), "=d"(*h):"c"(msr));
}
void wrmsr(uint32_t msr, uint32_t l, uint32_t h) {
    asm volatile ("wrmsr;"::"a"(l), "d"(h), "c"(msr));
}