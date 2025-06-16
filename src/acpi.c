#include "header/acpi.h"


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
static uint8_t* disasm_aml(uint8_t* code, size_t code_size) {
    static uint8_t code[MAX_LEN] = {0, };
    size_t ptr = 0;
    uint8_t tmp[0xff] = {0, };
    
    putcode(code, "DefinitionBlock (", &ptr, ptr);
    for (size_t i = 0; i < code_size; i++) {
        
    }

    return code;
}
void putcode(uint8_t* code, const char* str, size_t* ptr, size_t pos) {
    *(ptr) = pos;
    for (size_t i = 0; str[i] != 0; i++) {
        code[*(ptr)++] = str[i];
    }
}
uint8_t ops2char(uint8_t ops) {
    uint8_t digit[10] = "0123456789";
    uint8_t name[]
}