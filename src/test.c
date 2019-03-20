#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "simd_crc.h"

#if defined(_MSC_VER)
#include <windows.h>
#else
#include <sys/time.h>
#endif

int main()
{
    int i;
    int8_t *llr = (int8_t *)malloc(sizeof(int8_t) * 8448);
    uint8_t *bits = (uint8_t *)malloc(sizeof(uint8_t) * 8448 / 8);

    for (i = 0; i < 8448; i++)
        llr[i] = i % 2 ? -2 : 2;

    fast_decide_avx512(llr, 8448, bits);

    for (i = 0; i < 10; i++)
        printf("%02X\n", bits[i]);

    free(llr);
    free(bits);

    return 0;
}