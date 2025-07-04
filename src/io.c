#include "header/io.h"


void outb(uint8_t origin, uint16_t dest) {
    __asm__ volatile ("movb %0, %%al; movw %1, %%dx; outb %%al, %%dx"::"r"(origin), "r"(dest));
}
uint8_t inb(uint16_t origin) {
    uint8_t ret;
    __asm__ volatile ("movw %1, %%dx; inb %%dx, %%al; movb %%al, %0":"=r"(ret):"r"(origin));
    return ret;
}
void io_wait() {
    __asm__ volatile ("movb $0x00, %al; movw $0x0080, %dx; outb %al, %dx");
}
uint64_t rdmsr(uint32_t msr) {
    uint32_t l, h;
    uint64_t ret;
    __asm__ volatile ("rdmsr":"=a"(l), "=d"(h):"c"(msr));
    ret = ((uint64_t)h << 32) + l;
    return ret;
}
void wrmsr(uint32_t msr, uint64_t value) {
    uint32_t l = (uint32_t)(value & 0xffffffff);
    uint32_t h = (uint32_t)(value >> 32);
    __asm__ volatile ("wrmsr;"::"a"(l), "d"(h), "c"(msr));
}