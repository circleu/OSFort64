#ifndef __KERNEL_H
#define __KERNEL_H

#include <stdint.h>
#include "osfrt_print.h"
#include "mem.h"
#include "bitmap.h"
#include "pageframealloc.h"
#include "paging.h"
#include "pagetablemgr.h"
#include "def.h"
#include "func.h"
#include "gdt.h"
#include "int.h"
#include "bootinfo.h"
#include "osfrt_acpi.h"
#include "proc.h"
#include "syscall.h"


extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

#endif