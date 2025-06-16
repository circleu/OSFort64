#include "header/func.h"


uint64_t pow(uint64_t a, uint64_t b) {
    if (b == 0) return 1;

    uint64_t temp = a;
    while (b - 1 > 0) {
        temp *= a;
        b--;
    }

    return temp;
}
char* itoa(uint64_t num) {
    static char temp[21];
    int k = 0;

    for (int i = 0; i < 21; i++) temp[i] = 0;

    if (num >= pow(10, 19)) temp[0] = num / pow(10, 19) + 48;
    else k++;

    for (uint64_t i = pow(10, 18); num < i; i /= 10) k++;
    
    if (k == 20) {
        temp[0] = '0';
        return (char*)temp;
    } 

    for (int i = 0; i < 20 - k; i++) {
        uint64_t j; j = pow(10, 19 - i - k);
        temp[i] = num % (j * 10) / j + 48;
    }

    return (char*)temp;
}
char* hex(uint64_t num) {
    static char temp[17];
    char hex[17] = "0123456789ABCDEF";

    for (int i = 0; i < 17; i++) temp[i] = 0;

    for (int i = 0; i < 16; i++) {
        uint8_t byte; byte = (num & ((uint64_t)0xf << i * 4)) >> i * 4;

        temp[15 - i] = hex[byte];
    }

    return (char*)temp;
}
char* hexbyte(uint8_t num) {
    static char temp[3];
    char hex[17] = "0123456789ABCDEF";

    for (int i = 0; i < 3; i++) temp[i] = 0;

    for (int i = 0; i < 2; i++) {
        uint8_t byte; byte = (num & ((uint64_t)0xf << i * 4)) >> i * 4;

        temp[1 - i] = hex[byte];
    }

    return (char*)temp;
}
bool strncmp(uint8_t* x, uint8_t* y, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (x[i] != y[i]) return FALSE;
    return TRUE;
}