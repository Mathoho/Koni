/**
  * @file loader.h
  */

#ifndef _LOADER_H
#define _LOADER_H

#define ADD_T 0
#define REM_T 1 
#define EX_T 2 
#define IS_T 3 

struct transaction_t {
    /* The type of transaction */
    int type;
    /* The stock source, if applicable */
    int stock_src;
    /* The stock destination, if applicable */
    int stock_dest;
    /* The number of items of type stock_src/stock_dest to add, remove or exchange */
    int num_items;
    /* A pointer to the next instruction */
    struct transaction_t *next;
};

/**
 * @brief Creates an add transaction and adds it to a linked list of transactions 
 */
struct transaction_t* load_add(int type, int stock_dest, int num_items, struct transaction_t* transaction_list); 

/**
 * @brief Creates a remove transaction and adds it to a linked list of transactions 
 */
struct transaction_t* load_remove(int type, int stock_src, int num_items, struct transaction_t* transaction_list); 

/**
 * @brief Creates an exchange transaction and adds it to a linked list of transactions 
 */
struct transaction_t* load_exchange(int type, int stock_src, int stock_dest, int num_items, struct transaction_t* transaction_list); 

/**
 * @brief Creates an instock transaction and adds it to a linked list of transactions 
 */
struct transaction_t* load_instock(int type, int stock_src, struct transaction_t* transaction_list); 

#endif
