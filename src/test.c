#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "simd_crc.h"
#include "crc.h"

#if defined(_MSC_VER)
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define CRC_24A 0x1864CFB
#define CRC_16 0x11021

int main()
{
#if defined(_MSC_VER)
    LARGE_INTEGER num;
    long long start, end, freq;
#else
    struct timeval start, end;
    long timeuse;
#endif
    double run_time_dec;
    double run_time_crc;
    int i, res;
    int B = 8448;
    int loop = 1e6;
    int bn = (B - 1) / 8 + 1;
    int8_t *llr = (int8_t *)malloc(sizeof(int8_t) * B);
    uint8_t *bits = (uint8_t *)malloc(sizeof(uint8_t) * bn);
    for (i = 0; i < B; i++)
        llr[i] = i % 2 ? -2 : 2;
    for (i = 0; i < bn; i++)
        bits[i] = i;

    nr5g_crc_t crc_t;
    nr5g_crc_init(&crc_t, CRC_24A, 24);

    // printf("original:\t%d\n", nr5g_crc_checksum_byte(&crc_t, bits, B));
    // printf("int16:\t%d\n", nr5g_crc_checksum_16(&crc_t, bits, B));
    // exit(0);

#if defined(_MSC_VER)
    QueryPerformanceFrequency(&num);
    freq = num.QuadPart;
    QueryPerformanceCounter(&num);
    start = num.QuadPart;
#else
    gettimeofday(&start, NULL);
#endif

    for (i = 0; i < loop; i++)
        fast_compress_avx512(llr, B, _MM_CMPINT_LT, bits);

#if defined(_MSC_VER)
    QueryPerformanceCounter(&num);
    end = num.QuadPart;
    run_time_dec += (double)(end - start) / freq;
#else
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    run_time_dec += (double)timeuse / 1000000.0;
#endif

        fast_extend_avx512(bits, B/8, llr);

#if defined(_MSC_VER)
    QueryPerformanceFrequency(&num);
    freq = num.QuadPart;
    QueryPerformanceCounter(&num);
    start = num.QuadPart;
#else
    gettimeofday(&start, NULL);
#endif

    for (i = 0; i < loop; i++)
        res = nr5g_crc_checksum_byte(&crc_t, bits, B);

#if defined(_MSC_VER)
    QueryPerformanceCounter(&num);
    end = num.QuadPart;
    run_time_crc += (double)(end - start) / freq;
#else
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    run_time_crc += (double)timeuse / 1000000.0;
#endif

    printf("run_time_dec:\t%lfms\n", run_time_dec * 1e3);
    printf("run_time_crc:\t%lfms\n", run_time_crc * 1e3);
    printf("CRC Result:\t%d\n", res);
    free(llr);
    free(bits);

    return 0;
}