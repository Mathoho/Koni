/**
 * @file loader.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "syntax.h"
#include "loader.h"

/**
 * @brief Creates an add transaction and adds it to a linked queue of transactions 
 */
struct transaction_t* load_add(int type, int stock_dest, int num_items, struct transaction_t* transaction_queue) {
    struct transaction_t *transaction = NULL;
    
    // TODO: allocate memory for transaction and then 
    //       assign the applicable parameter (or else a 0 or NULL) to each of its fields
    transaction = malloc(sizeof(struct transaction_t));
    transaction->type = type;
    transaction->stock_dest = stock_dest;
    transaction->num_items = num_items;
    transaction->stock_src = 0;
    transaction->next = NULL;
    // TODO: if the transaction_queue is empty, assign transaction to transaction_queue
    //       else if the transaction_queue, is not empty, add transaction to the end of the linked list
    if(transaction_queue == NULL){
        transaction_queue = transaction;
    }else {
        struct transaction_t *temp = transaction_queue;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = transaction;
    }
    //printf("***\033[22;32mLoading add transaction not supported yet: please implement\033[0m\n");
    
    return transaction_queue;
}

/**
 * @brief Creates a remove transaction and adds it to a linked queue of transactions 
 */
struct transaction_t* load_remove(int type, int stock_src, int num_items, struct transaction_t *transaction_queue) {
    struct transaction_t *transaction = NULL;

    // TODO: allocate memory for transaction and then 
    //       assign the applicable parameter (or else a 0 or NULL) to each of its fields
    transaction = malloc(sizeof(struct transaction_t));
    transaction->type = type;
    transaction->stock_dest = 0;
    transaction->num_items = num_items;
    transaction->stock_src = stock_src;
    transaction->next = NULL;
    
    // TODO: if the transaction_queue is empty, assign transaction to transaction_queue
    //       else if the transaction_queue, is not empty, add transaction to the end of the linked list 
    if(transaction_queue == NULL){
        transaction_queue = transaction;
    }else {
        struct transaction_t *temp = transaction_queue;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = transaction;
    }
    
    //printf("***\033[22;32mLoading remove transaction not supported yet: please implement\033[0m\n");
    return transaction_queue;
} 

/**
 * @brief Creates an exchange transaction and adds it to a linked queue of transactions 
 */
struct transaction_t * load_exchange(int type, int stock_src, int stock_dest, int num_items, struct transaction_t* transaction_queue) {
    struct transaction_t *transaction = NULL;
    // TODO: allocate memory for transaction and then 
    //       assign the applicable parameter (or else a 0 or NULL) to each of its fields
    transaction = malloc(sizeof(struct transaction_t));
    transaction->type = type;
    transaction->stock_dest = stock_dest;
    transaction->num_items = num_items;
    transaction->stock_src = stock_src;
    transaction->next = NULL;
    
    // TODO: if the transaction_queue is empty, assign transaction to transaction_queue
    //       else if the transaction_queue, is not empty, add transaction to the end of the linked list 
    if(transaction_queue == NULL){
        transaction_queue = transaction;
    }else {
        struct transaction_t *temp = transaction_queue;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = transaction;
    }
    //printf("***\033[22;32mLoading exchange transaction not supported yet: please implement\033[0m\n");
    return transaction_queue;
}

/**
 * @brief Creates an instock transaction and adds it to a linked queue of transactions 
 */
struct transaction_t* load_instock(int type, int stock_src, struct transaction_t* transaction_queue) {
    struct transaction_t *transaction = NULL;
    // TODO: allocate memory for transaction and then 
    //       assign the applicable parameter (or else a 0 or NULL) to each of its fields
    transaction = malloc(sizeof(struct transaction_t));
    transaction->type = type;
    transaction->stock_dest = 0;
    transaction->num_items = 0;
    transaction->stock_src = stock_src;
    transaction->next = NULL;
    // TODO: if the transaction_queue is empty, assign transaction to transaction_queue
    //       else if the transaction_queue is not empty, add transaction to the end of the linked list 
    if(transaction_queue == NULL){
        transaction_queue = transaction;
    }else {
        struct transaction_t *temp = transaction_queue;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = transaction;
    }
    //printf("***\033[22;32mLoading instock transaction not supported yet: please implement\033[0m\n");
    return transaction_queue;
}
