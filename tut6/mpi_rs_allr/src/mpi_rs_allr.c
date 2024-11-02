/* File:     mpi_allreduce.c
 *
 * Purpose:  Implement an algorithm that uses allreduce to 
 *           compute the sum of all the rank values. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "logger.h"

/*-------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
   int rank = 0, ranksum = 0;
	//int comm_szk = 0;

	/* TODO: Start up MPI */
	MPI_Init(NULL,NULL);

	/* TODO: Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	/* TODO: Execute a broadcast reduction operation to compute the rank sum */
	MPI_Allreduce(&rank, &ranksum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	
	/* TODO: If you're process 0, call the log_result function */

	if (rank == 0)
	{
		log_result(rank, ranksum);
	}
	
	/* TODO: Shut down MPI */
	MPI_Finalize();
	return 0;
}
