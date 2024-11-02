#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

double nth_tri_serial(int n, int nt);
double nth_tri_critical(int n, int nt);
double nth_tri_manual_private(int n, int nt);
double nth_tri_reduction(int n, int nt);

#define DEBUG 0 

int main(int argc, char *argv[])
{
    double sum = 0;
    int n, nt = 1, choice;

    if (argc != 4){
        printf("Incorrect Invocation, use: omp_nth_tri <N> <number of threads> \
                <choice: 1-serial, 2-critical, 3-manual reduction, 4-reduction> \n");
        return 0;
    } else {
        n = atoi(argv[1]);
        nt = atoi(argv[2]);
        choice = atoi(argv[3]);
    }

    if (n < 0){
        printf("N should not be negative");
        return 0;
    }

    switch (choice) {
        case 2: 
            sum = nth_tri_critical(n, nt);  break;
        case 3: 
            sum = nth_tri_manual_private(n, nt); break;
        case 4:  
            sum = nth_tri_reduction(n, nt); break;
        default:  
            sum = nth_tri_serial(n, nt);
    }
    printf("Sum = %lf", sum);
}

/**
 * nth_tri_critical calculates the nth triangular number in parallel 
 * - using a team of <nt> threads and a parallel for loop 
 * - synchronises access to sum using the critical directive 
 * - times the execution of the function using both clock and omp_get_wtime(); 
 * - prints the times measured by both clock() and omp_get_wtime() 
 * - returns: the nth triangular number 
 */
double nth_tri_critical(int n, int nt) {
    
//TODO: implement this function as described in the comment
clock_t start_time, end_time;
start_time = clock();
double sum;
float start, finish;
#pragma omp parallel num_threads(nt)
{
    if (omp_get_thread_num() == 0)
    {
        start = omp_get_wtime();
    }
    
    int i;
    #pragma omp for
    for (i = 1; i < n+1; i++)
    {
        #pragma omp critical
        {
            sum = sum + i;
        }
    }
    if (omp_get_thread_num() == 0)
    {
        finish = omp_get_wtime();
    }
    
}
end_time = clock();
printf("Elapsed time = %.12lf seconds\n", (finish - start));
printf("time= %f \n", difftime(end_time, start_time)/CLOCKS_PER_SEC); 
return sum; 
}

/**
 * nth_tri_critical calculates the nth triangular number in parallel
 * - using a team of <nt> threads and a parallel for loop 
 * - uses a local_sum variable for a more efficient implementation 
 * - synchronises access to global_sum using the critical directive 
 * - times the execution of the function using both clock and omp_get_wtime(); 
 * - prints the times measured by both clock() and omp_get_wtime() 
 * - returns: the nth triangular number 
 */
double nth_tri_manual_private(int n, int nt) {

//TODO: implement this function as described in the comment 
double global_sum=0.0; 
float start, finish;
clock_t start_time, end_time;
start_time = clock();
#pragma omp parallel  num_threads(nt)
{
    if (omp_get_thread_num() == 0)
    {
        start = omp_get_wtime();
    }

    int i;
    double local_sum = 0.0;
    #pragma omp for
    
    for (i = 1; i < n+1; i++)
    {
        local_sum = local_sum + i;
    }
    #pragma omp critical
    {
        global_sum = global_sum+local_sum;
    }
    if (omp_get_thread_num() == 0)
    {
        finish = omp_get_wtime();
    }
    
}
end_time = clock();
printf("Elapsed time = %.12lf seconds\n", finish - start);
printf("time= %f \n", difftime(end_time, start_time)/CLOCKS_PER_SEC); 

    return global_sum;

}

/**
 * nth_tri_reduction calculates the nth triangular number in parallel 
 * - using a team of <nt> threads, a parallel for loop and the reduction clause 
 * - times the execution of the function using both clock and omp_get_wtime(); 
 * - prints the times measured by both clock() and omp_get_wtime() 
 * - returns: the nth triangular number 
 */
double nth_tri_reduction(int n, int nt) {

//TODO: implement this function as described in the comment 
float start, finish; 
clock_t start_time, end_time;
start_time = clock();
double sum = 0.0;
#pragma omp parallel  num_threads(nt) reduction(+:sum)
{
    if (omp_get_thread_num() == 0)
    {
        start = omp_get_wtime();
    }
    int i;
    #pragma omp for
    for (i = 1; i < n+1; i++)
    {
        sum = sum+i;
    }
    if (omp_get_thread_num() == 0)
    {
        finish = omp_get_wtime();
    }
    
}
end_time = clock();
printf("Elapsed time = %.12lf seconds\n", (finish - start));
printf("time= %f \n", difftime(end_time, start_time)/CLOCKS_PER_SEC); 

return sum;
}

/**
 * nth_tri_serial calculates and returns the nth triangular number 
 * - using a for loop
 * - times the execution of the function using clock()
 * - prints the time measured by clock()
 */
double nth_tri_serial(int n, int nt) {
    clock_t start = 0, end = 0;
    double sum = 0;

    start = clock();
    for(int i = 1; i <= n; i++) {
        sum += i;
    }
    end = clock();

    printf("Serial for loop: clock() measured %.3lf\n",difftime(end,start)/CLOCKS_PER_SEC);
    return sum;
}
