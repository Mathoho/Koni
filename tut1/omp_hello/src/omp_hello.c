
//TODO Round 1: Include all the necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) {
  int my_rank = 0;  //thread id
  int nt = 1;       //number of threads to create
 

  //TODO Round 2: If a command-line argument is provided, assign it to nt 
  if(argc > 0) {
    nt = argc;
  }
 

  //TODO Round 1: Parallelise this code as described on SunLearn 
  #pragma omp parallel num_threads(4) 
  {

  //Round 2: Update the #pragma omp parallel directive by adding the num_threads clause  
    char* string = malloc(32*sizeof(char));
    sprintf(string, "Hello from thread %d of %d\n", my_rank, nt);
    printf("%s", string);
  }

  return 0;
}
