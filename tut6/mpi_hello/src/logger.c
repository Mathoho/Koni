/**
  * Do not change this file 
  */

#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

/* Logging a result */
void log_result(int rank, int res) { 
    printf("Proc %d: the final result is %d\n", rank, res);
}

/* Logging a message */
void log_msg(int rank, char *msg) { 
    printf("Proc %d: %s\n", rank, msg);
}
