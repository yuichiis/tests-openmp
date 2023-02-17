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


#define ROWS 10000
#define COLS 10000
float data1[ROWS*COLS];
float data2[COLS];



extern void test_vect(long n, float *vec, float a) {
    int i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        vec[i] += a * a * 2 + 4;
    }
}

extern void test_simd(long n, float *x, float a, float *y) {
    int i;
    for(i=0;i<n;i++) {
        x[i] += a * y[i];
    }
}

extern void test_complex(long m, long n, float *x, float a, float *y) {
    int i;
    #pragma omp parallel for
    for(i=0;i<m;i++) {
        test_simd(n,&x[i*n],a,y);
    }
}

static void test_simd2(long n, float *x, float a, float *y) {
    int i;
    for(i=0;i<n;i++) {
        x[i] += a * y[i];
    }
}

extern void test_complex2(long m, long n, float *x, float a, float *y) {
    int i;
    #pragma omp parallel for
    for(i=0;i<m;i++) {
        test_simd2(n,&x[i*n],a,y);
    }
}

int main()
{
    /* code */
    float a = 1.0;

#ifdef _OPENMP
    printf("num threads=%d\n",omp_get_num_threads());
    printf("max threads=%d\n",omp_get_max_threads());
#endif

    printf("init data1\n");
    for(int i=0;i<ROWS*COLS; i++) {
        data1[i] = (float)i/1000;
    }
    printf("init data2\n");
    for(int i=0;i<COLS; i++) {
        data2[i] = (float)i/1000;
    }

    printf("start test_vect\n");
    dwStart = GetTickCount();
    test_vect(ROWS*COLS, data1, a);
    printf("%ld milliseconds\n", GetTickCount() - dwStart);

    printf("start test_simd\n");
    dwStart = GetTickCount();
    test_simd(COLS, data1, a, data2);
    printf("%ld milliseconds\n", GetTickCount() - dwStart);

    printf("start test_complex\n");
    dwStart = GetTickCount();
    test_complex(ROWS, COLS, data1, a, data2);
    printf("%ld milliseconds\n", GetTickCount() - dwStart);

    printf("start test_complex\n");
    dwStart = GetTickCount();
    test_complex2(ROWS, COLS, data1, a, data2);
    printf("%ld milliseconds\n", GetTickCount() - dwStart);

    printf("Core dump check");
    return 0;
}
