/**
  * @file log.c
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "log.h"
#include <omp.h>

/**
 * @Brief: Successful add message printed 
 */
void log_add_items(int num_items, int stock_dest, int total) {
    printf("Thread %2d: Add %3d items of type %3d %24s %4d \n", \
        omp_get_thread_num(), num_items, stock_dest, "-- #items now", total);
}

/**
 * @Brief: Error msg printed: stock type does not exist
 */
void log_add_items_inv(int num_items, int stock_dest) {
        printf("\033[22;31mThread %2d: Add %3d items of type %2d %60s \033[0m\n", \
            omp_get_thread_num(), num_items, stock_dest, "-- Error: store and/or stock type does not exist");
}


/**
 * @Brief: Successful remvoe message printed and logged  
 */
void log_rem_items(int num_items, int stock_src, int total) {
    printf("Thread %2d, Remove %4d items of type %3d %20s %4d\n", omp_get_thread_num(), \
        num_items, stock_src, "-- #items now", total);
} 

/**
 * @Brief: Error msg printed: Remove #items not available  
 */
void log_rem_items_none(int num_items, int stock_src, int num_available) { 
    printf("\033[22;32mThread %2d, Remove %4d items of type %3d %15s %4d %s \033[0m\n", \
        omp_get_thread_num(), num_items, stock_src, "-- only ", \
        num_available, "available -- failed");
}

/**
 * @Brief: Error msg printed: Remove stock type does not exist  
 */
void log_rem_items_inv(int num_items, int stock_src) {
    printf("\033[22;31mThread %2d, Remove %4d items of type %3d %55s \033[0m\n", \
        omp_get_thread_num(), num_items, stock_src, "-- Error: store and/or stock type does not exist");
}
    
/**
 * @Brief: Successful exchange message printed and logged 
 */
void log_ex(int num_items, int stock_from, int stock_to, int total) {
    printf("Thread %2d, Exchange %3d items of type %2d for %2d -- #items of type %2d now %4d\n", \
        omp_get_thread_num(), num_items, stock_from, stock_to, stock_from, total);
}

/**
 * @Brief: Error msg printed: Exchange #items not available 
 */
void log_ex_items_none(int num_items, int stock_from, int stock_to, int num_available) { 
    printf("\033[22;32mThread %2d, Exchange %3d items of type %2d to %2d %9s %4d available -- failed \033[0m\n", \
        omp_get_thread_num(), num_items, stock_from, stock_to, "-- only ", num_available);
}

/**
 * @Brief: Error msg printed: Exchange stock type does not exist 
 */
void log_ex_inv(int num_items, int stock_from, int stock_to) {
    printf("\033[22;31mThread %2d, Exchange %3d items of type %2d to %2d %50s", \
        omp_get_thread_num(), num_items, stock_from, stock_to, \
        "-- Error: store or stock type does not exist\033[0m\n");
}

/**
 * @Brief: Successful balance request message printed and logged 
 */
void log_instock(int stock_src, int num_available) {
    printf("Thread %2d, #items %5s %2d %28s %4d\n", \
        omp_get_thread_num(), "of type ", stock_src, "-- #items ", num_available);
}

/**
 * @Brief: Error msg printed: Stock type not available 
 */
void log_instock_inv(int stock_src) { 
    printf("\033[22;31mThread %2d, #items of type %2d %63s \033[0m\n", \
        omp_get_thread_num(), stock_src, "-- Error: store or stock type does not exist");
}
