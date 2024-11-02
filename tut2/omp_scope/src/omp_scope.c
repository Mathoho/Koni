#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char *argv[]){
    if(argc != 4){
        return 0;
}
    int nt = atoi(argv[1]);
    int value = atoi(argv[2]);
    int scope = atoi(argv[3]);
    printf("Before: value = %d\n", value);
    if(scope == 1){
        #pragma omp parallel num_threads(nt)
        {
            int my_rank = omp_get_thread_num();
            printf("Inside parallel region on thread %d: value = %d\n", my_rank, value); 
        }
        printf("After: value = %d\n", value);
    }else{
        #pragma omp parallel num_threads(nt) private(value)
        {
            int my_rank = omp_get_thread_num();
            printf("Inside parallel region on thread %d: value = %d\n", my_rank, value); 
        }
        printf("After: value = %d\n", value);
    }
    
    return 0;
}
