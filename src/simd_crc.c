#include "simd_crc.h"

#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include <string.h>

int fast_decide_avx512(const int8_t *input, int len, uint8_t *output)
{
    int i, j;
    int units = len / 64;
    int remain = len % 64;
    __m512i zero_zmm = _mm512_setzero_si512();
    __m512i input_zmm;
    __mmask64 output_m64;
    const int8_t *pi = input;
    __mmask64 *po = (__mmask64 *)output;

    for (i = 0; i < units; ++i)
    {
        input_zmm = _mm512_loadu_epi8(pi);
        output_m64 = _mm512_cmp_epi8_mask(input_zmm, zero_zmm, 1);
        _store_mask64(po, output_m64);
        pi += 64;
        ++po;
    }
    if (remain)
    {
        units = (remain - 1) / 8 + 1;
        input_zmm = _mm512_loadu_epi8(pi);
        output_m64 = _mm512_cmp_epi8_mask(input_zmm, zero_zmm, 1);
        memcpy(po, &output_m64, units);
    }

    return 0;
}