#ifndef SIMD_CRC
#define SIMD_CRC

#include <stdint.h>
#include <immintrin.h>

int fast_decide_avx512(const int8_t *input, int len, uint8_t *output);

int fast_extend_avx512(const uint8_t *input, int len, uint8_t *output);

#endif