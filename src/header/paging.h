#ifndef __PAGING_H
#define __PAGING_H

#include <stdint.h>
#include "def.h"



typedef struct {
    bool p : 1;
    bool rw : 1;
    bool us : 1;
    bool pwt : 1;
    bool pcd : 1;
    bool a : 1;
    bool ignore0 : 1;
    bool ps : 1;
    bool ignore1 : 1;
    uint8_t avl0 : 3;
    uint64_t addr : 40;
    uint16_t avl1 : 11;
    bool xd : 1;
} PAGE_TABLE_ENTRY;
typedef struct {
    PAGE_TABLE_ENTRY entries[512];
}__attribute__((aligned(0x1000))) PAGE_TABLE;

#endif