#ifndef __OSFRT_ACPI_H
#define __OSFRT_ACPI_H

#include <stdint.h>
#include "def.h"
#include "func.h"
#include "osfrt_print.h"
#include "pageframealloc.h"
#include "pagetablemgr.h"

#define APIC_REGISTER_LAPIC_ID 0x20
#define APIC_REGISTER_LAPIC_VER 0x30
#define APIC_REGISTER_TPR 0x80
#define APIC_REGISTER_APR 0x90
#define APIC_REGISTER_PPR 0xa0
#define APIC_REGISTER_EOI 0xb0
#define APIC_REGISTER_RRD 0xc0
#define APIC_REGISTER_LDR 0xd0
#define APIC_REGISTER_DFR 0xe0
#define APIC_REGISTER_SPURIOUS 0xf0
#define APIC_REGISTER_ESR 0x280
#define APIC_REGISTER_ICRL 0x300
#define APIC_REGISTER_ICRH 0x310
#define APIC_REGISTER_LVT_TIMER 0x320
#define APIC_REGISTER_LVT_PMCR 0x340
#define APIC_REGISTER_LVT_LINT0 0x350
#define APIC_REGISTER_LVT_LINT1 0x360
#define APIC_REGISTER_TIMER_ICR 0x380
#define APIC_REGISTER_TIMER_DIV 0x3e0

#define MAX_LEN = 0xffff


typedef struct {
    uint8_t sign[4];
    uint32_t len;
    uint8_t rev;
    uint8_t checksum;
    uint8_t oem_id[6];
    uint8_t oem_table_id[8];
    uint32_t oem_rev;
    uint32_t creator_id;
    uint32_t creator_rev;
}__attribute__((packed)) SDT_HEADER;
typedef struct {
    uint8_t type; // 0
    uint8_t len; // 8
    uint8_t acpi_proc_uid;
    uint8_t apic_id;
    uint32_t flags;
}__attribute__((packed)) APIC_PROCESSOR_LOCAL_APIC;
typedef struct {
    uint8_t type; // 1
    uint8_t len; // 12
    uint8_t io_apic_id;
    uint8_t reserved;
    uint32_t io_apic_addr;
    uint32_t global_sys_int_base;
}__attribute__((packed)) APIC_IO_APIC;
typedef struct {
    uint8_t type; // 2
    uint8_t len; // 10
    uint8_t bus;
    uint8_t src;
    uint32_t global_sys_int;
    uint16_t flags;
}__attribute__((packed)) APIC_INTERRUPT_SOURCE_OVERRIDE;
typedef struct {
    uint8_t type; // 3
    uint8_t len; // 8
    uint16_t flags;
    uint32_t global_sys_int;
}__attribute__((packed)) APIC_NMI_SOURCE;
typedef struct {
    uint8_t type; // 4
    uint8_t len; // 6
    uint8_t acpi_proc_uid;
    uint16_t flags;
    uint8_t local_apic_lint;
}__attribute__((packed)) APIC_LOCAL_APIC_NMI;
typedef struct {
    uint8_t type; // 5
    uint8_t len; // 12
    uint16_t reserved;
    uint64_t local_apic_addr;
}__attribute__((packed)) APIC_LOCAL_APIC_ADDRESS_OVERRIDE;
typedef struct {
    uint8_t type; // 6
    uint8_t len; // 16
    uint8_t io_apic_id;
    uint8_t reserved;
    uint32_t global_sys_int_base;
    uint64_t io_sapic_addr;
}__attribute__((packed)) APIC_IO_SAPIC;
typedef struct {
    uint8_t type; // 7
    uint8_t len; // variable
    uint8_t acpi_proc_id;
    uint8_t local_sapic_id;
    uint8_t local_sapic_eid;
    uint8_t reserved[3];
    uint32_t flags;
    uint32_t acpi_proc_uid_value;
}__attribute__((packed)) APIC_LOCAL_SAPIC;
typedef struct {
    uint8_t type; // 8
    uint8_t len; // 16
    uint16_t flags;
    uint8_t int_type;
    uint8_t proc_id;
    uint8_t proc_eid;
    uint8_t io_sapic_vector;
    uint32_t global_sys_int;
    uint32_t platform_int_src_flags;
}__attribute__((packed)) APIC_PLATFORM_INTERRUPT_SOURCE;
typedef struct {
    uint8_t type; // 9
    uint8_t len; // 16
    uint16_t reserved;
    uint32_t x2_apic_id;
    uint32_t flags;
    uint32_t apic_proc_uid;
}__attribute__((packed)) APIC_PROCESSOR_LOCAL_X2_APIC;
typedef struct {
    APIC_PROCESSOR_LOCAL_APIC proc_local_apic;
    APIC_IO_APIC io_apic;
    APIC_INTERRUPT_SOURCE_OVERRIDE int_src_override;
    APIC_NMI_SOURCE nmi_src;
    APIC_LOCAL_APIC_NMI local_apic_nmi;
    APIC_LOCAL_APIC_ADDRESS_OVERRIDE local_apic_addr_override;
    APIC_PROCESSOR_LOCAL_X2_APIC proc_local_x2_apic;
}__attribute__((packed)) APIC_STRUCTURE;
typedef struct {
    SDT_HEADER header;
    uint64_t other_sdt[32];
}__attribute__((packed)) XSDT;
typedef struct {
    SDT_HEADER header;
    uint32_t local_apic_addr;
    uint32_t flags;
    APIC_STRUCTURE apic_struct;
}__attribute__((packed)) MADT;
typedef struct {
    SDT_HEADER header;
    uint32_t firmware_ctrl;
    uint32_t dsdt;
    uint8_t reserved0;

    // there's more, but I want dsdt only
}__attribute__((packed)) FADT;
typedef struct {
    SDT_HEADER header;
}__attribute__((packed)) DSDT;

MADT* acpi_find_madt(XSDT* xsdt);
FADT* acpi_find_fadt(XSDT* xsdt);


#endif