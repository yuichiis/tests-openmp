#ifdef _OPENMP
#include <omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DATA_SIZE 100000
int main()
{
    /* code */
    double Max=-INFINITY;
    double Min=INFINITY;
    int LoopNum = DATA_SIZE;
    double Data[DATA_SIZE];

    printf("num threads=%d\n",omp_get_num_threads());
    printf("max threads=%d\n",omp_get_max_threads());
    for(int i=0;i<LoopNum; i++) {
        Data[i] = (double)i;
    }
    #pragma omp parallel
    {
        double TempMax=-INFINITY,TempMin=INFINITY;
        int i;
        #pragma omp for
        for(i=0;i<LoopNum;i++) {
            if(TempMax<Data[i]) TempMax=Data[i];
            if(TempMin>Data[i]) TempMin=Data[i];
        }
        #pragma omp critical
        {
            if(Max<TempMax) Max=TempMax;
            if(Min>TempMin) Min=TempMin;
        }
    }
    printf("min=%lf, max=%lf\n",Min,Max);
    return 0;
}
