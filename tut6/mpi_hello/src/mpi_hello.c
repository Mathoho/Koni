/* vim:
 * :se tw=2
 *
 * File: 
 * mpi_hello.c
 *
 * Purpose: 
 * A "hello world" program that uses MPI
 */
#include <stdio.h>
#include <string.h> 
#include <mpi.h>   
#include "logger.h"

#define MAX 100

int main(void) {
	char greeting[MAX];  
	int my_rank = 0, comm_sz = 0;

	/* TODO: Start up MPI */
	MPI_Init(NULL,NULL);
	/* TODO: Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	
	/* TODO: Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (my_rank != 0) {
		/* Create greeting message for process my_rank */
		sprintf(greeting, "Greetings from process %d of %d!", my_rank, comm_sz);
		/* TODO: Send message to process 0 */
		MPI_Send(greeting,strlen(greeting)+1,MPI_CHAR,0,0,MPI_COMM_WORLD);

	} else {
		sprintf(greeting, "Greetings from process %d of %d!", my_rank, comm_sz);
		/* TODO: Call log_msg once parallelised - do not change it */
		log_msg(my_rank, greeting); 

		/* TODO: Receive a message from each of the other processes and print it*/
		for (int q = 1; q < comm_sz; q++)
		{
			MPI_Recv(greeting,MAX,MPI_CHAR,q,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			log_msg(my_rank, greeting); 
		}
		
			/* TODO: Call log_msg once parallelised */
		        
	}
	
	/* TODO: Shut down MPI */
	MPI_Finalize();
	return 0;
}
