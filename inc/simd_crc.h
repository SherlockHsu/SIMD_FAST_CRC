#ifndef SIMD_CRC
#define SIMD_CRC

#include <stdint.h>
#include <immintrin.h>

/*
0: OP := _MM_CMPINT_EQ
1: OP := _MM_CMPINT_LT
2: OP := _MM_CMPINT_LE
3: OP := _MM_CMPINT_FALSE
4: OP := _MM_CMPINT_NE
5: OP := _MM_CMPINT_NLT
6: OP := _MM_CMPINT_NLE
7: OP := _MM_CMPINT_TRUE
*/
int fast_compress_avx512(const int8_t *input, int len, int imm8, uint8_t *output);

int fast_extend_avx512(const uint8_t *input, int len, uint8_t *output);

#endif