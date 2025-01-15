#ifndef __EFIMEMORY_H
#define __EFIMEMORY_H

#include <stdint.h>


// Came from efidef.h
typedef struct {
    uint32_t            type;           // Field size is 32 bits followed by 32 bit pad
    uint32_t            pad;
    uint64_t            phys_addr;  // Field size is 64 bits
    uint64_t            vir_addr;   // Field size is 64 bits
    uint64_t            num_pages;  // Field size is 64 bits
    uint64_t            attr;      // Field size is 64 bits
} EFI_MEMORY_DESCRIPTOR;

#endif