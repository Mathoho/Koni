#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define FALSE 0
#define TRUE 1 
#define COUNT_TASKS 1 

static int isprime(int n);
void par_tasks_numprimes(int n, int num_threads);
void par_for_numprimes(int n, int num_threads); 
static void init_counters(int num_threads);
static void inc_counter();
static void print_counter(); 
static void print_nprimes(int n, int nprimes); 

#ifdef COUNT_TASKS
int* task_counter;  //an array storing a counter for each of the <num_threads> threads
#endif


int main(int argc, char* argv[]) {
    int num_iter = 1, num_threads = 1;
    int work_sharing_type = 1;

//  TODO: 
//     Read in 3 arguments: num_iter, num_threads, work_sharing_type
//     If the incorrect number of arguments are given print the following usage message:
//         "Usage: omp_tasks <number of tasks> <number of threads>, <work_sharing_type: 1-task, 2-for>"
    if (argc != 4){
        printf("Usage: omp_tasks <number of tasks> <number of threads>, <work_sharing_type: 1-task, 2-for>");
        return 0;
    }
    num_iter = atoi(argv[1]);
    num_threads = atoi(argv[2]);
    work_sharing_type = atoi(argv[3]);

    init_counters(num_threads);

    if (work_sharing_type == 1) {
        par_tasks_numprimes(num_iter, num_threads); 
    } else if (work_sharing_type == 2) {
        par_for_numprimes(num_iter, num_threads); 
    } else {
        printf("Unknown work sharing type and/or task\n");
    }

    return 0;
}

/*
 * isprime is a naïve prime calculation that returns true (false) when n is prime (not prime)
 */ 
static int isprime(int n) {
    int i = 0;
    inc_counter();    

    for(i = 2; i <= (int) (sqrt((double) n)); i++) {
        if (n % i == 0) {
          return FALSE;
        }
    }

    return n > 1;
}

/*
 * par_tasks_numprimes counts the number of primes from 1 to n 
 * in parallel using the #pragma omp task worksharing construct
 */ 
void par_tasks_numprimes(int n, int num_threads) {
    int nprimes = 0;

    //TODO: parallelise this code using tasks
    #pragma omp parallel num_threads(num_threads) reduction(+:nprimes)
    { 
        
        #pragma omp single 
        {
            for(int i = 0; i < n; i++) {
            
                if (isprime(i))
                {
                    #pragma omp task
                    {   
                        
                        nprimes+=isprime(i);
                       
                    }  
                }
            } 
        }   
        #pragma omp taskwait
        print_counter(); // this should be called by each thread after all its tasks have been completed 
    }
     //print_counter(); 
    print_nprimes(n, nprimes); // this should be called only once after the calculation is finished 
}

/**
 *  par_for_numprimes calculates the number of primes from 1 to n 
 *  in parallel using a parallel for loop
 */
void par_for_numprimes(int n, int num_threads) {
    int nprimes = 0; 

    //TODO: parallelise this code using the for loop worksharing construct 
    #pragma omp parallel num_threads(num_threads) firstprivate(nprimes)
    {
        #pragma omp for
        for (int i = 1; i < n; i++) {
            nprimes += isprime(i);
            
        }
           print_counter(); // this should be called by each thread after all its tasks have been completed
       // #pragma omp cr
        //nprimes = nprimes + nprm;
    }
    
    print_nprimes(n, nprimes); // this should be called only once after the calculation is finished 
}

/** 
 * init_counters allocates memory for and initialises the global <task_counter> array 
 * - Note that you can combine these two steps using calloc
 */
static void init_counters(int num_threads) {
#ifdef COUNT_TASKS
    // TODO: implement this function
    task_counter = (int *) calloc(num_threads,sizeof(int));
#endif
}

/** 
 * inc_counter increments the task counter of the thread calling this function
 * - The calling thread should be in a parallel region
 */
static void inc_counter() {
#ifdef COUNT_TASKS
    // TODO: implement this function
    task_counter[omp_get_thread_num()]++;
    //COUNT_TASKS = COUNT_TASKS+1;    
#endif
}

/** 
 * print_counter prints: "Thread i executed x tasks"
 * - where x is the number of tasks thread i executed 
 * - The calling thread should be in a parallel region
 */
static void print_counter() {
#ifdef COUNT_TASKS
    // TODO: implement this function
    printf("Thread %d executed %d tasks\n",omp_get_thread_num(),task_counter[omp_get_thread_num()]);
#endif
}

static void print_nprimes(int n, int nprimes) {
#ifdef DEBUG 
    printf("The number of primes up to %d is %d\n", n, nprimes);
#endif
}
