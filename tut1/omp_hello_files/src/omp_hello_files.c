//TODO: Include all the necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


void write_to_file(int my_rank, char* msg);

int main(int argc, char* argv[]) {
    int my_rank = 0, nt;
    
    

    /*TODO: 
     * - assign the command-line argument to nt 
     *      - arguments are in string format, so use atoi()
     * - if the wrong number of arguments were given, print the following usage message
     *   printf("Usage: ./omp_hello_files <number of threads>\n"); 
     */

    if(argc != 2) {
        printf("Usage: ./omp_hello_files <number of threads>\n");
        return 0;
    }
    nt = atoi(argv[0]);

    //TODO: parallelise this code
    # pragma omp parallel num_threads(nt)
    {
    my_rank = omp_get_thread_num();
    char* msg = malloc(32*sizeof(char));
    sprintf(msg, "Hello World from thread %d of %d\n", my_rank, omp_get_num_threads());
    write_to_file(my_rank, msg);
    }
    return 0;
} 

/* TODO:
 * - Open thread *my_rank*'s logfile for appending
 *   - The name of the file should be p%d.log (use sprintf())
 *     so that thread 0 writes to p0.log, thread 1 to p1.log, ... 
 * - Write *msg* to the logfile (use fprintf())
 * - Test for errors and print appropriate error messages to avoid segfaults
 */
void write_to_file(int my_rank, char* msg) {
    char* filename = NULL;
    filename = malloc(32*sizeof(char));
    FILE* file_ptr = NULL;
    sprintf(filename,"p%d.log",omp_get_thread_num());
    file_ptr = fopen(filename,"w");
    fprintf(file_ptr, "%s",msg);
}
