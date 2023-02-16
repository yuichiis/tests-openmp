#ifdef _OPENMP
#include <omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#if _MSC_VER
#include <windows.h>
volatile DWORD dwStart;
volatile int global = 0;
#else
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
volatile uint64_t dwStart;
volatile int global = 0;
/// Returns the number of ticks since an undefined time (usually system startup).
static uint64_t GetTickCount()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64_t)(ts.tv_nsec / 1000000) + ((uint64_t)ts.tv_sec * 1000ull);
}
#endif


#define DATA_SIZE 10000000
float data[DATA_SIZE];
float data[DATA_SIZE];



extern void test_vect(long n, float *vec, float a) {
    int i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        vec[i] += a * a * 2 + 4;
    }
}

extern void test_simd(long n, float *vec, float a) {
    int i;
    #pragma omp simd
    for(i=0;i<n;i++) {
        vec[i] += a * a * 2 + 4;
    }
}

int main()
{
    /* code */
    float a = 1.0;

    printf("num threads=%d\n",omp_get_num_threads());
    printf("max threads=%d\n",omp_get_max_threads());

    for(int i=0;i<DATA_SIZE; i++) {
        data[i] = (float)i;
    }

    dwStart = GetTickCount();
    test_vect(DATA_SIZE, data, a);
    printf("%ld milliseconds\n", GetTickCount() - dwStart);

    dwStart = GetTickCount();
    test_simd(DATA_SIZE, data, a);
    printf("%ld milliseconds\n", GetTickCount() - dwStart);

    printf("Core dump check");
    return 0;
}
