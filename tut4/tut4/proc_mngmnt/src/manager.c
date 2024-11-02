/**
 * @mainpage Process Simulation
 *
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proc_structs.h"
#include "proc_syntax.h"
#include "logger.h"
#include "manager.h"

#define LOWEST_PRIORITY -1 // Use INT_MAX if 0 has the highest priority

int num_processes = 0;

/**
 * The queues as required by the spec
 */
static pcb_queue_t terminatedq;
static pcb_queue_t waitingq;
static pcb_queue_t readyq;
static bool_t readyq_updated;

void schedule_fcfs();
void schedule_rr(int quantum);
void schedule_pri_w_pre();
bool_t higher_priority(int, int);

void execute_instr(pcb_t *proc, instr_t *instr);
void request_resource(pcb_t *proc, instr_t *instr);
void release_resource(pcb_t *proc, instr_t *instr);
bool_t acquire_resource(pcb_t *proc, char *resource_name);

bool_t check_for_new_arrivals();
void move_proc_to_wq(pcb_t *pcb, char *resource_name);
void move_waiting_pcbs_to_rq(char *resource_name);
void move_proc_to_rq(pcb_t *pcb);
void move_proc_to_tq(pcb_t *pcb);
void enqueue_pcb(pcb_t *proc, pcb_queue_t *queue);
pcb_t *dequeue_pcb(pcb_queue_t *queue);

char *get_init_data(int num_args, char **argv);
char *get_data(int num_args, char **argv);
int get_algo(int num_args, char **argv);
int get_time_quantum(int num_args, char **argv);
void print_args(char *data1, char *data2, int sched, int tq);

void print_avail_resources(void);
void print_alloc_resources(pcb_t *proc);
void print_queue(pcb_queue_t queue, char *msg);
void print_running(pcb_t *proc, char *msg);
void print_instructions(instr_t *instr);

/* my METHODs*/
int comparePcb(pcb_t *first,pcb_t *second);
int compareResource(resource_t *first, resource_t *second);
struct pcb_t *get_highest_priority_pro();
void *dequeue_pcb_from_link(pcb_queue_t *queue,pcb_t *proc);

/*RESOURCES GLOBAL
struct pcb_t *pcb;*/

int main(int argc, char **argv)
{
    char *data1 = get_init_data(argc, argv);
    char *data2 = get_data(argc, argv);
    int scheduler = get_algo(argc, argv);
    int time_quantum = get_time_quantum(argc, argv);
    print_args(data1, data2, scheduler, time_quantum);

    pcb_t *initial_procs = NULL;
    if (strcmp(data1, "generate") == 0)
    {
#ifdef DEBUG_MNGR
        printf("****Generate processes and initialise the system\n");
#endif
        initial_procs = init_loader_from_generator();
    }
    else
    {
#ifdef DEBUG_MNGR
        printf("Parse process files and initialise the system: %s, %s \n", data1, data2);
#endif
        initial_procs = init_loader_from_files(data1, data2);
    }

    /* schedule the processes */
    if (initial_procs)
    {
        num_processes = get_num_procs();
        init_queues(initial_procs);
        printf("***********Scheduling processes************\n");
        schedule_processes(scheduler, time_quantum);
        dealloc_data_structures();
    }
    else
    {
        printf("Error: no processes to schedule\n");
    }
    /*pcb = malloc(num_processes*sizeof(struct pcb_t));*/

    return EXIT_SUCCESS;
}

/**
 * @brief The linked list of loaded processes is moved to the readyqueue.
 *        The waiting and terminated queues are intialised to empty
 * @param cur_pcb: a pointer to the linked list of loaded processes
 */
void init_queues(pcb_t *cur_pcb)
{
    readyq.first = cur_pcb;
    for(cur_pcb = readyq.first; cur_pcb->next != NULL; cur_pcb = cur_pcb->next);
    readyq.last = cur_pcb;
    readyq_updated = FALSE;

    waitingq.last = NULL;
    waitingq.first = NULL;
    terminatedq.last = NULL;
    terminatedq.first = NULL;

#ifdef DEBUG_MNGR
    printf("-----------------------------------");
    print_queue(readyq, "Ready");
    printf("\n-----------------------------------");
    print_queue(waitingq, "Waiting");
    printf("\n-----------------------------------");
    print_queue(terminatedq, "Terminated");
    printf("\n");
#endif /* DEBUG_MNGR */
}

/**
 * @brief Schedules each instruction of each process
 *
 * @param type The scheduling algorithm to use
 * @param quantum The time quantum for the RR algorithm, if used.
 */
void schedule_processes(schedule_t sched_type, int quantum)
{
    printf("TODO: Implement two schedulers: a Priority based scheduler and either a Round Robin (RR) scheduler or a FCFS scheduler \n");

    switch (sched_type)
    {
    case PRIOR:
        schedule_pri_w_pre();
        break;
    case RR:
        schedule_rr(quantum);
        break;
    case FCFS:
       schedule_fcfs();
        break;
    default:
        break;
    }
}

/** Schedules processes using priority scheduling with preemption */
void schedule_pri_w_pre()
{
    /* TODO: Implement */
    /*pcb_t *point = readyq.first;
    pcb_t *higher_pr = readyq.first;
    //pcb_t *last_before_pointer = readyq.last;
    //TODO this will only run one process with a highest pririty
    //TODO must i do all processes at one call
    while (point != NULL)
    {
        if (higher_priority(point->priority,higher_pr->priority))
        {
            higher_pr = point;  
        }
        point = point->next;
        
    }
    /*TODO i have found the highest priority now i execute it
    instr_t *instr = higher_pr->process_in_mem->first_instr;
    while (instr != NULL)
    {
        higher_pr->state = RUNNING;
        execute_instr(higher_pr,instr);
        
        /*check priority again
        pcb_t *pointer = readyq.first;
        while (pointer != NULL)
        {
           if (higher_priority(pointer->priority,higher_pr->priority))
            {
                higher_pr->state = READY;
                /*do not remove from ready queue
                higher_pr = pointer;
                instr = higher_pr->process_in_mem->first_instr;  
            }
           pointer = pointer->next;
        
        }
        instr = instr->next;   
    }
    */
    /*new PRIORITY CODE*/
    //int break_from_instr_loop = 0;
    while (readyq.first != NULL)
    {
        pcb_t *pointer = readyq.first;
        //while (pointer != NULL)
        //{
            pcb_t *higher_priority = get_highest_priority_pro();
            higher_priority->state = RUNNING;
            pcb_t *removed = dequeue_pcb_from_link(&readyq,higher_priority);
            instr_t *instr_exc = higher_priority->next_instruction;
            while (instr_exc != NULL)
            {
                execute_instr(higher_priority,instr_exc);
                if (check_for_new_arrivals() == TRUE)
                {
                    pcb_t *new_higher_priority = get_highest_priority_pro();
                    if (comparePcb(higher_priority, new_higher_priority) == 0)
                    {
                        /*not equal*/
                        higher_priority->next_instruction = instr_exc;
                        move_proc_to_rq(higher_priority); 
                        
                        instr_exc = new_higher_priority->next_instruction;
                        execute_instr(higher_priority,instr_exc);
                        //break_from_instr_loop = 1;
                    }else {
                        /*just execute the current instruction*/
                        execute_instr(higher_priority,instr_exc);
                    }   
                }
                instr_exc = instr_exc->next; 
            }
            if (instr_exc == NULL)
            {
                /*process terminated remove from readyqueue*/
                move_proc_to_tq(higher_priority);
                dequeue_pcb_from_link(&readyq,higher_priority);
            }
            
            
        //}
        
    }
    
    
    //TODO set it to terminated but how will i know it was succesfully executed and 
    //TODO i also have to remove it from the runnung que but again how will i know it has terminated
}

/** Schedules processes using FCFS scheduling */
void schedule_fcfs()
{
    /* TODO: Implement if chosen as 2nd option */
    //TODO this implements all processes in the queue must i do one process
    pcb_t *pointer = readyq.first;
    while (pointer != NULL)
    {
        instr_t *instr = pointer->next_instruction;
        pcb_t *removed = dequeue_pcb_from_link(&readyq,pointer);
        while (instr != NULL)
        {
            execute_instr(pointer,instr);
            bool_t newarvl = check_for_new_arrivals();
            instr = instr->next;
        }
        pointer = pointer->next;
    }
    
    
}

/**
 * Schedules processes using the Round-Robin scheduler.
 *
 * @param[in] quantum time quantum
 */
void schedule_rr(int quantum)
{
    /* TODO: Implement if chosen as 2nd option */
    /*quantum is the number of instructions to execute
    //TODO does this mean on one process or the number of processes to execute
    pcb_t *pointer = readyq.first;
    while (pointer != NULL)
    { 
        instr_t *instruction = pointer->process_in_mem->first_instr;
        int i;
        for (i = 0; i < quantum; i++)
        {
           while (instruction != NULL)
           {
                execute_instr(pointer,instruction);
                instruction = instruction->next;  
           }
        }
        /*if not terminated put in the readyq
        if (i == quantum && pointer->state != TERMINATED)
        {
            pointer->state = READY;
            enqueue_pcb(pointer,&readyq);
        }
        pointer = pointer->next; 
    }*/

    /*NEW IMPLEMENTATION*/
    pcb_t *pointer = readyq.first;
    while (pointer != NULL)
    {
        instr_t *instruction = pointer->next_instruction;
        pcb_t *removed = dequeue_pcb_from_link(&readyq,pointer);
        int i;
        for (i = 0; i < quantum; i++)
        {
           while (instruction != NULL)
           {
                execute_instr(pointer,instruction);
                bool_t newarvl = check_for_new_arrivals();
                instruction = instruction->next;  
           }
           if (instruction == NULL)
           {
                break;   
           }
           
        }
        if (i <= quantum && instruction == NULL)
        {
            /* TERMINATED WITHIN THE TIME QUANTUM */
            pointer->state = TERMINATED;
            move_proc_to_tq(pointer);
            dequeue_pcb_from_link(&readyq,pointer);
        }else if(i == quantum && instruction != NULL){
            pointer->next_instruction = instruction;
            pointer->state = READY;
            /*Do not remove from ready que*/
        }
        
        pointer = pointer->next;
    }
    

}

/**
 * Executes a process instruction.
 *
 * @param[in] pcb
 *     processs for which to execute the instruction
 * @param[in] instr
 *     instruction to execute
 */
void execute_instr(pcb_t *pcb, instr_t *instr)
{

    if (instr != NULL)
    {
        switch (instr->type)
        {
        case REQ_OP:
            request_resource(pcb, instr);
            break;
        case REL_OP:
            release_resource(pcb, instr);
            break;
        default:
            break;
        }
    }
    else
    {
        printf("Error: No instruction to execute\n");
    }

#ifdef DEBUG_MNGR
    printf("-----------------------------------");
    print_running(pcb, "Running");
    printf("\n-----------------------------------");
    print_queue(readyq, "Ready");
    printf("\n-----------------------------------");
    print_queue(waitingq, "Waiting");
    printf("\n-----------------------------------");
    print_queue(terminatedq, "Terminated");
    printf("\n");
#endif
}

/**
 * @brief Handles the request resource instruction.
 *
 * Executes the request instruction for the process. The function loops
 * through the list of resources and acquires the resource if it is available.
 * If the resource is not available the process is moved to the waiting queue
 *
 * @param current The current process for which the resource must be acquired.
 * @param instruct The request instruction
 */
void request_resource(pcb_t *cur_pcb, instr_t *instr)
{
    printf("TODO: implement a function that can execute the request resource instruction\n");
    

    char *current_resource_name = instr->resource_name;
   
    cur_pcb->next_instruction = instr;
    

    /*go through the list of the loaded resources and find out if the resource is avilable or not*/
    int available = 0;
    resource_t *pointer = get_available_resources();
    while (pointer != NULL && available == 0)
    {
        if (strcmp(current_resource_name, pointer->name) == 0)
        {
            if (pointer->available == YES)
            {
                available+=1;         
                pointer->available = NO;
                break;
            } 
        }
        pointer = pointer->next;
    }
    
    /*if it is available load to pcb resources*/
    if (available > 0){
        resource_t *resource = malloc(sizeof(struct resource_t));
        resource->name = instr->resource_name;
        resource->available = YES;
        
        if (cur_pcb->resources == NULL)
        {
            cur_pcb->resources = resource;
        }else
        {
            resource_t *temp = cur_pcb->resources;
             while (temp->next != NULL){
            temp = temp->next;
        }
        resource->next = NULL;
        temp->next = resource;
        }
        
        // If acquired
        log_request_acquired(cur_pcb->process_in_mem->name, instr->resource_name);
    } else if (available == 0){
      move_proc_to_wq(cur_pcb,current_resource_name);
    }      
}

/**
 * @brief Acquires a resource for a process.
 *
 * @param[in] process
 *     process for which to acquire the resource
 * @param[in] resource
 *     resource name
 * @return TRUE if the resource was successfully acquire_resource; FALSE otherwise
 */
bool_t acquire_resource(pcb_t *cur_pcb, char *resource_name)
{
    printf("TODO: implement a function that can assign resource_name to cur_pcb if the resource is available and mark it as unavailable in the resources list\n");

    /*making it unavailable in resource list*/
    /*go through the list of the loaded resources and find out if the resource is avilable or not*/

    int available = 0;
    resource_t *rp = get_available_resources();
    while (rp != NULL && available == 0)
    {
        if (strcmp(resource_name, rp->name)== 0)
        {
            if (rp->available == YES)
            {
                available+=1;
                /*making it unavailable*/
                rp->available = NO;
                break;
            } 
        }
        rp = rp->next;
    }

/*assign thr name*/
    if (available > 0) {
        resource_t *temp = cur_pcb->resources;
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next->name = resource_name;
        temp->next->available = NO;
        return TRUE;
    }
    return FALSE;
}

/**
 * @brief Handles the release resource instruction.
 *
 * Executes the release instruction for the process. If the resource can be
 * released the process is ready for next execution. If the resource can not
 * be released the process waits
 *
 * @param current The process which releases the resource.
 * @param instruct The instruction to release the resource.
 */
void release_resource(pcb_t *pcb, instr_t *instr)
{ 
    if (pcb->resources == NULL)
    {
        return;
    }
    char *curr_resource_name = instr->resource_name;
    int release = 0;
    
    //int available_resources = 0;
    printf("TODO: Implement a function that can release a resource and mark it available in the resources list");
    printf("If successful call log_release_released, else call log_release_error \n");
    /*loop through the resource list of the pcb and make it available*/
   
    
    resource_t *temp = pcb->resources;
    while (temp != NULL)
    {
        if (temp->name == curr_resource_name)
        {
            temp->available = YES;
            release = 1;
            /*HERE is the place of the resource i should release*/
        }
        temp = temp->next;
    }
    if (release == 1)
    {
        pcb->next_instruction = instr;
    }

    move_waiting_pcbs_to_rq(curr_resource_name);
             
    if (release == 1){
        // successful release
        log_release_released(pcb->process_in_mem->name, instr->resource_name);
    }else {
        // resource not assigned to process
        log_release_error(pcb->process_in_mem->name, instr->resource_name);
    }
}

/**
 * Add new process <code>pcb</code> to ready queue
 */
bool_t check_for_new_arrivals()
{
    pcb_t *new_pcb = get_new_pcb();

    if (new_pcb)
    {
        printf("New process arriving: %s\n", new_pcb->process_in_mem->name);
        move_proc_to_rq(new_pcb);
        return TRUE;
    }
    return FALSE;
}

/**
 * @brief Move process <code>pcb</code> to the ready queue
 *
 * @param[in] pcb
 */
void move_proc_to_rq(pcb_t *pcb)
{
    /* TODO: Implement */
    pcb->state = READY;
    if (readyq.first == NULL)
    {
        readyq.first = pcb;
        readyq.last = pcb;
        readyq.first->next = readyq.last;
        
    }else
    {
        readyq.last->next = pcb;
        readyq.last = pcb;
    }
    
    
   
    log_request_ready(pcb->process_in_mem->name);
}

/**
 * Move process <code>pcb</code> to waiting queue
 */
void move_proc_to_wq(pcb_t *pcb, char *resource_name)
{
    /* TODO: Implement */
    pcb->state = WAITING;
    pcb->next_instruction->type = REQ_OP;
    pcb->next_instruction->resource_name = resource_name; 
     /*adding current pcb to the waiting list*/
     if (waitingq.first == NULL)
     {
        waitingq.first = pcb;
        waitingq.last = pcb;
        waitingq.first->next = waitingq.last;
     }
     
    waitingq.last->next = pcb;
    waitingq.last = pcb;
    log_request_waiting(pcb->process_in_mem->name, resource_name);
}

/**
 * Move process <code>pcb</code> to terminated queue
 *
 * @param[in] pcb
 */
void move_proc_to_tq(pcb_t *pcb)
{
    /* TODO: Implement */
    pcb->state = TERMINATED;
    if (terminatedq.first == NULL)
    {
        terminatedq.first = pcb;
        terminatedq.last = pcb;
        terminatedq.first->next = terminatedq.last;
    }else{
        terminatedq.last->next = pcb;
        terminatedq.last = pcb;
    }
    
    
    log_terminated(pcb->process_in_mem->name);
}

/**
 * Moves all processes waiting for resource <code>resource_name</code>
 * from the waiting queue to the readyq queue.
 *
 * @param[in]   resource
 *     resource name
 */
void move_waiting_pcbs_to_rq(char *resource_name)
{
    /* TODO: Implement */
     /*Loop through wait queue full of pcbs
        /*if the pcb in the wait que's next intruction type was a [req] and the [name] was the name of the
        resource just released it should be removed from the waiting queue and then
        moved to the ready queue*/
    pcb_t *point = waitingq.first;
        pcb_t *prev = NULL;
        while (point != NULL)
        {
            if (point->next_instruction->type == REQ_OP && strcmp(point->next_instruction->resource_name,resource_name) == 0)
            {
                /* if it is the head*/
                if (point == waitingq.first)
                {
                    waitingq.first = point->next;
                    point->state = READY;
                    enqueue_pcb(point,&readyq);
                    free(point);
                    point = waitingq.first;
                }else{
                    /*if it is not the head make prev point to the pcb the removed pcb points to*/
                    prev->next = point->next;
                    point->state = READY;
                    enqueue_pcb(point,&readyq);
                    free(point);
                }
            }else{
                /*move to the next pcb in waitingq*/
                prev = point;
                point = point->next;
            }    
        }

}

/**
 * Enqueues process <code>pcb</code> to <code>queue</code>.
 *
 * @param[in] process
 *     process to enqueue
 * @param[in] queue
 *     queue to which the process must be enqueued
 */
void enqueue_pcb(pcb_t *pcb, pcb_queue_t *queue)
{
    /* TODO: Implement */

    /*if queue has nothing*/
    if (queue->first == NULL) {
        queue->first = pcb;
        queue->last = pcb;
        pcb->next = NULL;
    
    }else {
        queue->last->next = pcb;
        queue->last = pcb;
        pcb->next = NULL; 
    }
}

/**
 * Dequeues a process from queue <code>queue</code>.
 *
 * @param[in] queue
 *     queue from which to dequeue a process
 * @return dequeued process
 */
pcb_t *dequeue_pcb(pcb_queue_t *queue)
{
    /* TODO: Implement */

    /*if que has nothing*/
    pcb_t *removed_pcb = NULL;
        if (queue->first == NULL) {
        return NULL;
    }

    /*if there is only one element in the queue*/
    if (queue->first == queue->last) {
        return dequeue_pcb(queue);
    }

    /* Traverse the queue to find the second-to-last element*/
    pcb_t *crnt_pcb = queue->first;
    while (crnt_pcb->next != queue->last) {
        crnt_pcb = crnt_pcb->next;
    }

    /*Dequeue the last element*/
    removed_pcb = queue->last;
    queue->last = crnt_pcb;
    crnt_pcb->next = NULL;

    return removed_pcb;
}

/** @brief Return TRUE if pri1 has a higher priority than pri2
 *         where higher values == higher priorities
 *
 * @param[in] pri1
 *     priority value 1
 * @param[in] pri2
 *     priority value 2
 */
bool_t higher_priority(int pri1, int pri2)
{
    /* TODO: Implement */
    if (pri1 > pri2)
    {
        return TRUE;
    }
    
    //printf("Function higher_priority not implemented\n");
    return FALSE;
}

/**
 * @brief Inspect the waiting queue and detects deadlock
 */
struct pcb_t *detect_deadlock()
{
    /* TODO: Implement */
    printf("Function detect_deadlock not implemented\n");

    // if deadlock detected
    // log_deadlock_detected();

    return NULL;
}

/**
 * @brief Releases a processes' resources and sets it to its first instruction.
 *
 * Generates release instructions for each of the processes' resoures and forces
 * it to execute those instructions.
 *
 * @param pcb The process chosen to be reset and release all of its resources.
 *
 */
void resolve_deadlock(struct pcb_t *pcb)
{
    /* TODO: Implement */
    printf("Function resolve_deadlock not implemented\n");
}

/**
 * @brief Deallocates the queues
 */
void free_manager(void)
{
#ifdef DEBUG_MNGR
    print_queue(readyq, "Ready");
    print_queue(waitingq, "Waiting");
    print_queue(terminatedq, "Terminated");
#endif

#ifdef DEBUG_MNGR
    printf("\nFreeing the queues...\n");
#endif
    dealloc_pcb_list(readyq.first);
    dealloc_pcb_list(waitingq.first);
    dealloc_pcb_list(terminatedq.first);
}

/**
 * @brief Retrieves the name of a process file or the codename "generator" from the list of arguments
 */
char *get_init_data(int num_args, char **argv)
{
    char *data_origin = "generate";
    if (num_args > 1)
        return argv[1];
    else
        return data_origin;
}

/**
 * @brief Retrieves the name of a process file or the codename "generator" from the list of arguments
 */
char *get_data(int num_args, char **argv)
{
    char *data_origin = "generate";
    if (num_args > 2)
        return argv[2];
    else
        return data_origin;
}

/**
 * @brief Retrieves the scheduler algorithm type from the list of arguments
 */
int get_algo(int num_args, char **argv)
{
    if (num_args > 3)
        return atoi(argv[3]);
    else
        return 1;
}

/**
 * @brief Retrieves the time quantum from the list of arguments
 */
int get_time_quantum(int num_args, char **argv)
{
    if (num_args > 4)
        return atoi(argv[4]);
    else
        return 1;
}

/**
 * @brief Print the arguments of the program
 */
void print_args(char *data1, char *data2, int sched, int tq)
{
    printf("Arguments: data1 = %s, data2 = %s, scheduler = %s,  time quantum = %d\n", data1, data2, (sched == 0) ? "priority" : "RR", tq);
}

/**
 * @brief Print the names of the global resources available in the system in linked list order
 */
void print_avail_resources(void)
{
    struct resource_t *resource;

    printf("Available:");
    for (resource = get_available_resources(); resource != NULL; resource = resource->next)
    {
        if (resource->available == YES)
        {
            printf(" %s", resource->name);
        }
    }
    printf(" ");
}

/**
 * @brief Print the names of the resources allocated to <code>process</code> in linked list order.
 */
void print_alloc_resources(pcb_t *proc)
{
    struct resource_t *resource;

    if (proc)
    {
        printf("Allocated to %s:", proc->process_in_mem->name);
        for (resource = proc->resources; resource != NULL; resource = resource->next)
        {
            printf("%s", resource->name);
        }
        printf(" ");
    }
}

/**
 * @brief Print <code>msg</code> and the names of the processes in <code>queue</code> in linked list order.
 */
void print_queue(pcb_queue_t queue, char *msg)
{
    pcb_t *proc = queue.first;

    printf("%s:", msg);
    while (proc != NULL)
    {
        printf(" %s", proc->process_in_mem->name);
        proc = proc->next;
    }
    printf(" ");
}

/**
 * @brief Print <code>msg</code> and the names of the process currently running
 */
void print_running(pcb_t *proc, char *msg)
{
    printf("%s:", msg);
    if (proc != NULL)
    {
        printf(" %s", proc->process_in_mem->name);
    }
    printf(" ");
}

/**
 * @brief Print a linked list of instructions
 */
void print_instructions(instr_t *instr)
{
    instr_t *tmp_instr = instr;
    while (tmp_instr != NULL)
    {
        switch (tmp_instr->type)
        {
        case REQ_OP:
            printf("(req %s)\n", tmp_instr->resource_name);
            break;
        case REL_OP:
            printf("(rel %s)\n", tmp_instr->resource_name);
            break;
        case SEND_OP:
            printf("(send %s %s)\n", tmp_instr->resource_name, tmp_instr->msg);
            break;
        case RECV_OP:
            printf("(recv %s %s)\n", tmp_instr->resource_name, tmp_instr->msg);
            break;
        }
        tmp_instr = tmp_instr->next;
    }
}

/**
 * @brief it compares whether two pcbs are equal
 * returns 1 if they are equal and 0 otherwise
*/
int comparePcb(pcb_t *first,pcb_t *second)
{

    char *first_process_name = NULL;
    char *second_process_name = NULL;
    char *first_instr_name = NULL;
    char *second_instr_name = NULL;
    if (first == NULL && second == NULL)
    {
        return 1;
    }else if (first == NULL && second != NULL)
    {
        return 0;
    }else if (first != NULL && second == NULL)
    {
        return 0;
    }

    int equal = 0;
    int null = 0;
    int null_pr = 0;
    int null_instr = 0;
    /*process name*/
    if (first->process_in_mem->name == NULL &&  second->process_in_mem->name == NULL)
    {
        null+=1;
        null_pr+=1;
    }else if (first->process_in_mem->name == NULL &&  second->process_in_mem->name != NULL)
    {
        return 0;
    }else if (first->process_in_mem->name != NULL &&  second->process_in_mem->name == NULL)
    {
        return 0;
    }

    /*instruction name*/

    if (first->next_instruction->resource_name == NULL && second->next_instruction->resource_name == NULL)
    {
        null +=1;
        null_instr+=1;
    }else if (first->next_instruction->resource_name == NULL && second->next_instruction->resource_name != NULL)
    {
        return 0;
    }else if (first->next_instruction->resource_name != NULL && second->next_instruction->resource_name == NULL)
    {
        return 0;
    }

    if (null == 2)
    {
        return 1;
    }else if (null == 1 && null_pr == 0)
    {
        *first_process_name = first->process_in_mem->name;
        *second_process_name = second->process_in_mem->name; 
         /*process name equal*/
        if ((first_process_name == NULL) && (second_process_name == NULL)){
           equal +=1;
        }else if(first_process_name == NULL && second_process_name != NULL){
           /*do nothing*/ 
        }else if (first_process_name != NULL && second_process_name == NULL){
            /* do nothing*/
        }else if (strcmp(first_process_name,second_process_name) == 0){
            equal+=1;
        }
    }else if (null == 1 && null_instr == 0){
        /*come here if process is null */
        *first_instr_name = first->next_instruction->resource_name;
        *second_instr_name = second->next_instruction->resource_name;
        /*instruction name equal*/
         if ((first_instr_name == NULL) && (second_instr_name == NULL)){
             equal +=1;
        }else if(first_instr_name == NULL && second_instr_name != NULL){
         /*do nothing*/ 
        }else if (first_instr_name != NULL && second_instr_name == NULL){
           /* do nothing*/
        }else if (strcmp(first_instr_name,second_instr_name) == 0){
          equal+=1;
        }   
    }
    
    
    
    /*their states are equal*/
    if (first->state == second->state){
        equal+=1;
    }

    

    /*priority*/
    if (first->priority == second->priority)
    {
        equal+=1;
    }

    /*resources*/
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
 * returns 1 if they are equal and 0 otherwise
*/
int compareResource(resource_t *first, resource_t *second)
{
    if (first == NULL && second == NULL)
    {
        return 1;
    }else if (first == NULL && second != NULL)
    {
        return 0;
    }else if (first != NULL && second == NULL)
    {
        return 0;
    }
    
    
    
    int equal = 0;
    char *first_name = first->name;
    char *second_name = second->name;
    
    /*the resource name*/
    if ((first_name == NULL) && (second_name == NULL)){
        equal +=1;
    }else if(first_name == NULL && second_name != NULL){
       /*do nothing*/ 
    }else if (first_name != NULL && second_name == NULL){
        /* do nothing*/
    }else if (strcmp(first_name,second_name) == 0){
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
/**
 * @brief it sorts the readyq according to priorities
*/
struct pcb_t *get_highest_priority_pro(){
    pcb_t *pointer = readyq.first;
    pcb_t *higher_pr = readyq.first;
    while (pointer != NULL)
    {
       if (higher_priority(pointer->priority,higher_pr->priority))
        {
            higher_pr = pointer;
        }
        pointer = pointer->next;
        
    }
    return higher_pr;
}
/**
 * @brief
*/
void *dequeue_pcb_from_link(pcb_queue_t *queue, pcb_t *proc){
    pcb_t *removed_pcb = NULL;
    pcb_t *current = queue->first;
        pcb_t *prev = NULL;
        while (current != NULL)
        {
            if (comparePcb(current,proc) == 1)
            {
                if (prev == NULL)
                {
                    /*if it is the first pcb*/
                    queue->first = current->next;
                    queue->last = NULL;
                }else
                {
                    prev->next = current->next;
                }
                removed_pcb = current;
                //free(current);
               // return;
            }
            prev = current;
            current = current->next;
        }
}
