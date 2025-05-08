#ifndef __FUNC_H
#define __FUNC_H

#include <stdint.h>
#include "def.h"


uint64_t pow(uint64_t a, uint64_t b);
char* itoa(uint64_t num);
char* hex(uint64_t num);
bool strncmp(uint8_t* x, uint8_t* y, size_t n);

#endif