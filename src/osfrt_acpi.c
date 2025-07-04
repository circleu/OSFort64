#include "header/osfrt_acpi.h"


MADT* acpi_find_madt(XSDT* xsdt) {
    SDT_HEADER* header;
    for (size_t i = 0; i < sizeof(xsdt->other_sdt) / 8; i++) {
        header = (SDT_HEADER*)xsdt->other_sdt[i];
        if (strncmp(header->sign, "APIC", 4))
            return (void*)header;
    }
    return NULL;
}
FADT* acpi_find_fadt(XSDT* xsdt) {
    SDT_HEADER* header;
    for (size_t i = 0; i < sizeof(xsdt->other_sdt) / 8; i++) {
        header = (SDT_HEADER*)xsdt->other_sdt[i];
        if (strncmp(header->sign, "FACP", 4))
            return (void*)header;
    }
    return NULL;
}