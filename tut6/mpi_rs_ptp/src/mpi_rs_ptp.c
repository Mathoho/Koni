/* File:       
 *    mpi_rs_ptp.c
 *
 */
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#include "logger.h"

int main(void) {
   int my_rank = 0, ranksum = 0; /* My process rank */
   int comm_szk;
   /* TODO: Start up MPI */
   MPI_Init(NULL,NULL);

   /* TODO: Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_szk);
	
   /* TODO: Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

   if (my_rank != 0) { 
      /* TODO: Send my_rank to process 0 */
      MPI_Send(&my_rank,1,MPI_INTEGER,0,0,MPI_COMM_WORLD);

      /* TODO: Execute a broadcast to receive the rank sum 
      int temp_sum = my_rank;
		for (int q = 0; q < comm_szk; q++)
		{
         if (my_rank != q)
         {
            MPI_Recv(&ranksum,1,MPI_INTEGER,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			   temp_sum = temp_sum + ranksum;
         }	
		}*/
		//ranksum = temp_sum;
		//log_result(my_rank, ranksum);
   } else {  
	/* TODO: Initialise ranksum with the rank of process 0 */
      ranksum = my_rank;
      int temp_sum = my_rank;

	/* TODO: Receive the rank of each of the other processes and add it to ranksum */
      for (int q = 0; q < comm_szk; q++)
      {
         if (q != my_rank)
         {
            MPI_Recv(&ranksum,1,MPI_INTEGER,q,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			   temp_sum = temp_sum + ranksum;
         }
         
      }

      ranksum = temp_sum;

	/* TODO: Execute a broadcast to send ranksum to all the processes 
   for (int q = 0; q < comm_szk; q++)
   {
      if (q != my_rank)
      {
         MPI_Send(&my_rank,1,MPI_INTEGER,0,0,MPI_COMM_WORLD);
         
      }
      
   }*/
   
	log_result(my_rank, ranksum);
   }
log_result(my_rank, ranksum);
   /* Shut down MPI */
   MPI_Finalize();
   return 0;
}
