/**
 * @file parser.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "syntax.h"
#include "parser.h"
#include "loader.h"

#define NEW_LINE 0 
#define READING 1 
#define END_OF_FILE 2

FILE* open_transaction_file(char* filename);
struct transaction_t* read_transactions(FILE* fptr);
void read_add(FILE* fptr, char *line, int* stock_dest, int* num_items);
void read_remove(FILE* fptr, char *line,int* stock_src, int* num_items);
void read_exchange(FILE* fptr, char *line, int* stock_src, int* stock_dest, int* num_items);
void read_instock(FILE* fptr, char *line, int* stock_src);
int read_string(FILE* fptr, char* line);

/**
 * @brief Reads in a list of transactions from a specified file and stores the
 * transactions in appropriate data structures 
 * 
 * Reads the transaction.list file, 
 * - parses it, and 
 * - loads the transactions into a linked list of transactions. 
 *
 * @param filename The name of the transaction.list file 
 */
struct transaction_t* parse_transaction_file(char* filename) {
    FILE *fptr = NULL;
    struct transaction_t* transaction_list = NULL;

    fptr = open_transaction_file(filename);

    if (fptr == NULL) {
        printf("File %s could not be opened\n", filename);
    } else {
        transaction_list = read_transactions(fptr);
        fclose(fptr);
    } 
    return transaction_list;
}

/**
 * @brief Opens the file with filename and returns a pointer to the file.
 *
 * Opens a file, with filename, for read-only. If the file could not be
 * opened return NULL else return the pointer to the file.
 *
 * @param filename The name of the file to open
 *
 * @return A file pointer
 */
FILE* open_transaction_file(char* filename) {
    FILE *file = fopen(filename, "r"); 

    if (file == NULL) {
        file = NULL; 
    }

    return file;
}

/**
 * @brief Reads all the transactions and loads them
 *
 * @param fptr A pointer to the file from which to read.
 * @param line A pointer to a string read from file.
 *
 * @return transaction_list Returns the list of transactions 
 */
struct transaction_t* read_transactions(FILE* fptr) {
    char* trans_type;
    int stock_src, stock_dest;
    int num_items;
    int status;
    struct transaction_t* transaction_list = NULL;

    status = NEW_LINE; 
    
    /* reads the transaction name */
    trans_type = (char*) malloc(sizeof(char)*64); 
    while((status = read_string(fptr, trans_type)) != NEW_LINE && status != END_OF_FILE){
    if (strcmp(trans_type, ADD) == 0) {
        /* Reads the add stock item and num_items */
        read_add(fptr, trans_type, &stock_dest, &num_items);
        transaction_list = load_add(ADD_T,  stock_dest, num_items, transaction_list);
    }
    else if (strcmp(trans_type, REM) == 0) {
        /* Reads the remove stock item and num_items */
        read_remove(fptr, trans_type, &stock_src, &num_items);
        transaction_list = load_remove(REM_T,  stock_src, num_items, transaction_list);
    }
    else if (strcmp(trans_type, EX) == 0) {
        /* Reads the src and destination stock items and num_items */
        read_exchange(fptr, trans_type, &stock_src, &stock_dest, &num_items);
        transaction_list = load_exchange(EX_T,  stock_src, stock_dest, num_items, transaction_list);
    }
    else if (strcmp(trans_type, IS) == 0) {
        /* Reads the stock item */
        read_instock(fptr, trans_type, &stock_src);
        transaction_list = load_instock(IS_T, stock_src, transaction_list);
    }
    else {
        /* Executes on white spaces */
        /* Executes the while loop when encountering new lines and white spaces, */
        /* Exits the loop when end of file reached */
        if (strcmp(trans_type, "") != 0) {
            printf("\033[22;31mError: unknown transaction type: %s\n\033[0m", trans_type);
            break;
        }
    }
    trans_type = (char*) calloc(64, sizeof(char));
    }
    return transaction_list;
}

/**
 * @brief Reads the item number and num_items of an add instruction 
 *
 * Uses the read_string function 
 *
 * @param fptr A pointer to the file from which to read.
 * @param stock_dest A pointer to an integer read from the file 
 */
void read_add(FILE* fptr, char* line, int* stock_dest, int* num_items) {
    read_string(fptr, line); 
    *stock_dest = strtol(line, NULL, 10);
    read_string(fptr, line);
    *num_items = strtol(line, NULL, 10);
#ifdef DEBUG
    printf("add %d %d \n", *stock_dest, *num_items);
#endif
}

/**
 * @brief Reads the item number and num_items of a remove instruction 
 *
 * Uses the read_string function
 *
 * @param fptr A pointer to the file from which to read.
 * @param stock_src A pointer to an integer read from the file 
 */
void read_remove(FILE* fptr, char* line, int* stock_src, int* num_items) {
    read_string(fptr, line); 
    *stock_src = strtol(line, NULL, 10);
    read_string(fptr, line);
    *num_items = strtol(line, NULL, 10);
#ifdef DEBUG
    printf("remove %d %d \n", *stock_src, *num_items);
#endif
}


/**
 * @brief Reads the item numbers and num_items of an exchange instruction 
 *
 * Uses the read_string function 
 *
 * @param fptr A pointer to the file from which to read.
 * @param stock_src A pointer to an integer read from the file 
 * @param stock_dest A pointer to an integer read from the file 
 * @param num_items A pointer to an integer read from the file 
 */
void read_exchange(FILE* fptr, char* line, int* stock_src, int* stock_dest, int* num_items) {
    read_string(fptr, line); 
    *stock_src = strtol(line, NULL, 10);
    read_string(fptr, line);
    *stock_dest = strtol(line, NULL, 10);
    read_string(fptr, line);
    *num_items = strtol(line, NULL, 10);
#ifdef DEBUG
    printf("exchange %d %d %d \n", *stock_src, *stock_dest, *num_items);
#endif
}

/**
 * @brief Reads the item number of an in_stock instruction 
 *
 * Uses the read_string function to read the string and convert it to
 * a long int 
 *
 * @param fptr A pointer to the file from which to read.
 * @param line A pointer to a string read from the file 
 * @param stock_dest A pointer to an integer read from the file 
 */
void read_instock(FILE* fptr, char *line, int* stock_src) {
    read_string(fptr, line); 
    *stock_src = strtol(line, NULL, 10);
#ifdef DEBUG
    printf("in stock? %d \n", *stock_src);
#endif
}

/**
 * @brief Reads the next string.
 *
 * Reads the file character for character and constructs a string until a white
 * space or termination character is matched.
 *
 * @param fptr A pointer to the file from which to read.
 * @param line A pointer to memory where the string can be stored.
 *
 * return status The status indicates when the END_OF_FILE or NEW LINE has 
 * been reached.
 */
int read_string(FILE* fptr, char* line) {
    int index = 0;
    int ch = 0;
    int status = READING;
    
    ch = fgetc(fptr);
    while (ch != '\n' && ch != ' ') {
        if (ch == EOF) {
            status = END_OF_FILE;
            break;
        }
        line[index] = ch;
        index++;
        ch = fgetc(fptr);
        status = ( ch == '\n' ? NEW_LINE : READING );
    }
    line[index] = '\0';

    return status;
}


