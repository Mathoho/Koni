#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum {NEW = 0, READY, RUNNING, WAITING, TERMINATED} state_t;
typedef enum {REQ_OP = 0, REL_OP, SEND_OP, RECV_OP} instr_types_t; 
typedef enum {NO = 0, YES = 1} available_t; 
typedef enum {FALSE = 0, TRUE = 1} bool_t;

/** Each process has a linked list of instructions to execute.  */
typedef struct instr_t {
  instr_types_t type;
  char *resource_name; /* any resource, including a mailbox */
  char *msg; /* the message of a send or receive instruction */
  struct instr_t *next;
} instr_t;

/** A process process_in_mem stores the name and instructions of a process */
typedef struct process_in_mem_t {
  int number; 
  char *name;
  instr_t *first_instr;
} process_in_mem_t;

/** A type that represents a mailbox resource */
typedef struct mailbox_t {
  char *name;
  char *msg; 
  struct mailbox_t *next;
} mailbox_t;

/** A type that represents a resource */
typedef struct resource_t {
  char *name;
  available_t available; 
  struct resource_t *next;
} resource_t;

typedef struct pcb_t {
  struct process_in_mem_t *process_in_mem; /* process */
  int state; /* see enum state_t */
  struct instr_t *next_instruction;
  int priority; /* used for priority based scheduling */ 
  resource_t *resources; /* list of resources allocated to process */
  struct pcb_t *next;
} pcb_t;

/*METHODS*/
int compareResource(resource_t *first, resource_t *second);

/**
 * @brief it compares whether two pcbs are equal
*/
int comparePcb(pcb_t *first,pcb_t *second)
{
    int equal = 0;
    /*process name*/
    char *first_process_name = first->process_in_mem->name;
    char *second_process_name = second->process_in_mem->name;
    
    /*instruction name*/
    char *first_instr_name = first->next_instruction->resource_name;
    char *second_instr_name = second->next_instruction->resource_name;

    /*process name equal*/
    if (strcmp(first_process_name,second_process_name) == 0)
    {
        equal+=1;
    }

    /*their states are equal*/
    if (first->state == second->state){
        equal+=1;
    }

    /*instruction equal*/
    if (strcmp(first_instr_name,second_instr_name) == 0)
    {
        equal+=1;
    }

    if (first->priority == second->priority)
    {
        equal+=1;
    }
    
    if (compareResource(first->resources,second->resources) == 1)
    {
        equal+=1;   
    }
    
    if (equal == 5)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief it compares whether two resources are equal
*/
int compareResource(resource_t *first, resource_t *second)
{
    int equal = 0;
    char *first_name = first->name;
    char *second_name = second->name;
    
    /*the resource name*/
    if (strcmp(first_name,second_name) == 0)
    {
        equal+=1;    
    }

    if (first->available == second->available)
    {
        equal+=1;
    }
    
    if (equal == 2)
    {
        return 1;
    }
    
    return 0;
}

int main(){

    /*pcb_t*/
    pcb_t *fisrt_p = malloc(sizeof(struct pcb_t));
    pcb_t *second_p = malloc(sizeof(struct pcb_t));

    /*resource_t*/
    resource_t *fisrt_r = malloc(sizeof(struct resource_t));
    resource_t *second_r = malloc(sizeof(struct resource_t));

    /*process in memo*/
    process_in_mem_t *fisrt_pr = malloc(sizeof(struct process_in_mem_t));
    process_in_mem_t *second_pr = malloc(sizeof(struct process_in_mem_t));

    /*instr*/
    instr_t *fisrt_i = malloc(sizeof(struct instr_t));
    instr_t *second_i = malloc(sizeof(struct instr_t));

    /*FIRST ONE*/
    fisrt_i->msg = "my message";
    fisrt_i->resource_name = "R1";
    fisrt_i->type = REQ_OP;

    fisrt_pr->first_instr = fisrt_i;
    fisrt_pr->name = "P1";
    fisrt_pr->number = 1;

    fisrt_r->available = NO;
    fisrt_r->name = "R1";
    
    fisrt_p->next_instruction = fisrt_i;
    fisrt_p->priority = 1;
    fisrt_p->process_in_mem = fisrt_pr;
    fisrt_p->resources = fisrt_r;
    fisrt_p->state = NEW;
    
    /*SECOND ONE*/
    second_i->msg = "my message";
    second_i->resource_name = "R1";
    second_i->type = REQ_OP;

    second_pr->first_instr = second_i;
    second_pr->name = "P1";
    second_pr->number = 1;

    second_r->available = YES;
    second_r->name = "R1";
    
    second_p->next_instruction = second_i;
    second_p->priority = 1;
    second_p->process_in_mem = second_pr;
    second_p->resources = second_r;
    second_p->state = NEW;

    if (comparePcb(fisrt_p,second_p) == 1)
    {
        printf("they are equal\n");
    }else{
        printf("not equal\n");
    }
    

}
