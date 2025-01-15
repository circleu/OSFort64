#ifndef _IO_H
#define _IO_H

#include <stdint.h>

void outb(uint8_t origin, uint16_t dest);
uint8_t inb(uint16_t origin);
void io_wait();

#endif