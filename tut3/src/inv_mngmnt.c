/* 
 * @file: stock_mngmnt.c 
 * 
 * @description: A simplistic  inventory management system for a store with multiple online users 
 *  - the system keeps track of the #items in stock for each type of item
 *  - items of a certain type can be added, such as when stock is received from the warehouse 
 *  - items of a certain type can be removed, such as when an item is added to an online basket or bought 
 *  - items can be exchanged, i.e., it must be removed from the #items of one type and added to another 
 *
 * The execution command is in the run.sh, to run: . run.sh or bash run.sh or if marked executable ./run.sh
 * 
 * Currently, only one transaction list is read in and executed by one thread 
 * TODO: Parallelise this, so that 
 *       - multiple threads can each read a transaction list (one transaction list per thread)       
 *       - each thread can execute its own list of transactions in parallel with the other threads 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h> 
#include "loader.h"
#include "parser.h"
#include "log.h"
#include <pthread.h>

void add_item(int stock_id, int num_items);
void remove_item(int stock_id, int num_items);
void exchange_items(int stock_src, int stock_dest, int num_items);
void items_instock(int stock_id);
void free_transaction_queue(struct transaction_t* transaction_queue);//mine

void init_structures();
bool valid(int stock_id);
void usage(char* prog_name);
int get_num_files_from_args(int argc, char* argv[]);
char* get_fname_from_args(int argc, char* argv[], int thread_num);

const int num_stocks = 5;
int* stock;

omp_lock_t *locks;

/**------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int num_files = get_num_files_from_args(argc, argv);
    int thread_count = num_files;
    
    init_structures();
    
    if (thread_count == 1) {
        printf("\033[22;32m Warning: Only one thread reading one transaction file supported.\033[0m\n");
    }
    
      /*THE PARALLEL EXECUTION OF TRANSACTIONS*/
    
    int i;
    locks = malloc(num_stocks *sizeof(locks));
    for (i = 0; i < num_stocks; i++)
    {
        omp_init_lock(&locks[i]);   
    }
    #pragma omp parallel num_threads(thread_count)
    {   
        /* Parse file and load batch of transactions into transaction_queue */
        int index = omp_get_thread_num();
        struct transaction_t* transaction_queue;
        
        #pragma omp critical
        {
            char *filenamemine = get_fname_from_args(argc,argv,index);
            transaction_queue = parse_transaction_file(filenamemine);
        }
        
        if (transaction_queue == NULL) 
        { 
            printf("Something bad happened, parse_transaction_file(%s) returned no list\n", \
            get_fname_from_args(argc,argv,index));
            exit(0);
        } 
            
            while (transaction_queue != NULL) {
                if (transaction_queue->type == ADD_T){
                    add_item(transaction_queue->stock_dest, transaction_queue->num_items);
                } else if (transaction_queue->type == REM_T) {
                    remove_item(transaction_queue->stock_src, transaction_queue->num_items);
                } else if (transaction_queue->type == EX_T) {
                    exchange_items(transaction_queue->stock_src, transaction_queue->stock_dest, transaction_queue->num_items);
                } else if (transaction_queue->type == IS_T) {
                    items_instock(transaction_queue->stock_src);
                }
                //struct transaction_t temp = transaction_queue;
                transaction_queue = transaction_queue->next;
            }
            //#pragma omp_barrier
            #pragma omp critical
            {
                if (transaction_queue != NULL){
                    printf("do i free HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHmemo");
                    free_transaction_queue(transaction_queue);
                    if(transaction_queue != NULL){
                        free(transaction_queue);
                    }
                }  
            }                 
    }
    /* Execute batch of transactions 

    while (transaction_queue != NULL) {
        if (transaction_queue->type == ADD_T) {
            add_item(transaction_queue->stock_dest, transaction_queue->num_items);
        } else if (transaction_queue->type == REM_T) {
            remove_item(transaction_queue->stock_src, transaction_queue->num_items);
        } else if (transaction_queue->type == EX_T) {
            exchange_items(transaction_queue->stock_src, transaction_queue->stock_dest, transaction_queue->num_items);
        } else if (transaction_queue->type == IS_T) {
            items_instock(transaction_queue->stock_src);
        }
        transaction_queue = transaction_queue->next;
    }*/

    printf("    *********************************************************   \n");
    for (int i = 0; i < num_stocks; i++) {
        printf("#items of type stock[%2d] after completion of batch: %2d\n", i, stock[i]);
    }
    printf("    *********************************************************   \n");
    //omp_destroy_lock(&locks);
    
    /*DESTROYING LOCKS*/
    int k;
    for (k = 0; k < num_stocks; k++)
    {
        omp_destroy_lock(&locks[k]);
    } 
    free(locks); 
   
    return 0;   
}

/**--------------------------------------------------------------------
 * @brief add_item 
 *     Add num_items to stock of type stock_id 
 * @param stock_id: stock type 
 * @param num_items:  #items to add
 */
void add_item(int stock_id, int num_items)
{
    /*put the locks to stop the threads from aceesing stock[stock_id]
    at the same time do the same for all of them*/
    if (valid(stock_id)) {
        omp_set_lock(&locks[stock_id]);
        stock[stock_id] += num_items;
        log_add_items(num_items, stock_id, stock[stock_id]); 
        omp_unset_lock(&locks[stock_id]);
    } else {
        log_add_items_inv(num_items, stock_id);
    }
}

/**--------------------------------------------------------------------
 * @brief remove_item 
 *     If num_items available, subtract num_items from stock of type stock_id 
 * @param stock_id: stock type 
 * @param num_items: #items to remove
 */
void remove_item(int stock_id, int num_items)
{
    if (valid(stock_id)) {
        omp_set_lock(&locks[stock_id]);
       if (num_items <= stock[stock_id]) { 
            stock[stock_id] -= num_items;
            log_rem_items(num_items, stock_id, stock[stock_id]);
            omp_unset_lock(&locks[stock_id]);
        } else {
           log_rem_items_none(num_items, stock_id, stock[stock_id]);
           omp_unset_lock(&locks[stock_id]);
        }       
    } else {
        log_rem_items_inv(num_items, stock_id);
    }
}

/**--------------------------------------------------------------------
 * @brief exchange_items 
 *     If num_items available, subtract num_items 
 *     from the stock of type stock_src and add them to the stock of type stock_dest 
 * @param stock_src: Stock from which to remove num_items of type stock_id 
 * @param stock_dest: Stock to which num_items of type stock_id should be added 
 * @param num_items: #items to exchange
 */
void exchange_items(int stock_src, int stock_dest, int num_items) {
    
    if (valid(stock_src) && valid(stock_dest)) { 
        omp_set_lock(&locks[stock_src]);
        if (num_items <= stock[stock_src]) {  
            stock[stock_src] -= num_items;
            omp_unset_lock(&locks[stock_src]);
            omp_set_lock(&locks[stock_dest]);  
            stock[stock_dest] += num_items; 
            omp_unset_lock(&locks[stock_dest]); 
            omp_set_lock(&locks[stock_src]);
            log_ex(num_items, stock_src, stock_dest, stock[stock_src]);
            omp_unset_lock(&locks[stock_src]); 
        } else {
            log_ex_items_none(num_items, stock_src, stock_dest, stock[stock_src]);
            omp_unset_lock(&locks[stock_src]); 
        }
    } else {
        log_ex_inv(num_items, stock_src, stock_dest); 
    }
}

/**--------------------------------------------------------------------
 * @brief items_instock 
 *     Print the current number of items of type stock_id in the store 
 * @param stock_id: stock type 
 */
void items_instock(int stock_id)
{
    if (valid(stock_id)) {
        omp_set_lock(&locks[stock_id]);
        log_instock(stock_id, stock[stock_id]);
        omp_unset_lock(&locks[stock_id]); 
    } else {
        log_instock_inv(stock_id); 
    }
}

/**------------------------------------------------------------------
 * @brief  init_structures
 *     Allocated memory for and initialise the array of stock counts 
 *     Each entry holds the number of items for stock stock_id 
 */
void init_structures() {
    stock = (int*) calloc(num_stocks, sizeof(int));
    if (stock == NULL) { 
        fprintf(stderr,"Memory could not be allocated for stock exiting\n");
        exit(0);
    }
}

/**------------------------------------------------------------------
 * @brief  valid 
 *     Returns true if stock_id is a valid id
 * @param stock_id: stock type 
 *
 * Return true or false 
 */
bool valid(int stock_id) {
    //if (omp_get_thread_num() ==3 && stock_id > 5)
    //{
      //  stock_id = 4;
      //  printf("stock_id %d the num stock = %d\n",stock_id,num_stocks);
    //}
    
    return (stock_id < num_stocks); 
}

/**------------------------------------------------------------------
 * @brief  usage
 *     Print a message to show what the command line arguments should be, and terminate
 * @param: prog_name
 */
void usage (char* prog_name) {
    fprintf(stderr, "usage: %s <num_exansaction files> <transaction file> {<transaction file.txt>}\n", prog_name);
    exit(0);
}

/**------------------------------------------------------------------
 * @brief  get_num_files_from_args
 *     Get the 2nd command line arg
 * @param: argc
 * @param: argv
 *
 * Return thread_count 
 */
int get_num_files_from_args(int argc, char* argv[]) {

    if (argc < 3) usage(argv[0]);

    int num_files = strtol(argv[1], NULL, 10);
    if (num_files <= 0) usage(argv[0]);

    return num_files;

}  /* get_num_files_from_args */

/**------------------------------------------------------------------
 * @brief  get_fname_from_args
 *     Get the name of the transaction file for thread thread_num 
 * @param: argc
 * @param: argv
 *
 * Return the filename for thread id 
 */
char* get_fname_from_args(int argc, char* argv[], int id) {
    if (argc < (3 + id)) usage(argv[0]);
#   ifdef DEBUG
    printf("Returning argument 2 +%2d: transaction file = %s\n", id, argv[2+id]);
#   endif

    return argv[2 + id];

}  /* get_fname_from_args */

void free_transaction_queue(struct transaction_t* transaction_queue) {
    struct transaction_t* temp = transaction_queue;
    while (temp != NULL) {
        struct transaction_t* next = temp->next;
        free(temp);
        temp = next;
    }
}