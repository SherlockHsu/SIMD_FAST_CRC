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
#if defined(_MSC_VER)
    LARGE_INTEGER num;
    long long start, end, freq;
#else
    struct timeval start, end;
    long timeuse;
#endif
    double run_time;
    int i;
    int B = 8448;
    int loop = 1e4;
    int bn = (B - 1) / 8 + 1;
    int8_t *llr = (int8_t *)malloc(sizeof(int8_t) * B);
    uint8_t *bits = (uint8_t *)malloc(sizeof(uint8_t) * bn);
    for (i = 0; i < B; i++)
        llr[i] = i % 2 ? -2 : 2;

#if defined(_MSC_VER)
    QueryPerformanceFrequency(&num);
    freq = num.QuadPart;
    QueryPerformanceCounter(&num);
    start = num.QuadPart;
#else
    gettimeofday(&start, NULL);
#endif

    for (i = 0; i < loop; i++)
        fast_decide_avx512(llr, 8448, bits);

#if defined(_MSC_VER)
    QueryPerformanceCounter(&num);
    end = num.QuadPart;
    run_time += (double)(end - start) / freq;
#else
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    run_time += (double)timeuse / 1000000.0;
#endif

    printf("run_time:\t%lfus\n", run_time * 1e6);
    free(llr);
    free(bits);

    return 0;
}