#include <stdlib.h>
#include <stdio.h>
#include "gc.h"


//global list to keep track of allocated memory
extern struct mem_chunk * mem_list = NULL;

//defining counters for tracking variable in logfile
int initial_size;
int marked_one;
int totalDeletions;
int debug = 1;

/* A wrapper around malloc to keep track of dynamically allocated memory
 */
void *gc_malloc(int nbytes){
    //Making return address holder and a temporary address size holder
    size_t add_size = nbytes;
    void* address;

    if ((address = malloc(add_size)) == NULL){
        perror("malloc");
        exit(1);
    }

   //Making a new node in mem_list to record new address
    struct mem_chunk * new_mem;
    if ((new_mem = malloc(sizeof(struct mem_chunk))) == NULL) {
         perror("malloc");
         exit(1);
    }
    //setting flag to indicate that memory is in use and next is NULL
    new_mem->address = address;
    new_mem->in_use = USED;
    
    //adding new_mem to mem_list
    if (mem_list == NULL){
    	new_mem->next = NULL;
    } else{
	new_mem->next = mem_list;
    }
    //updating global mem_list and the allocations count
    mem_list = new_mem;
    
    //checking if debug is set
    if (debug == 1){
        printf("In gc_malloc: %lx\n", (unsigned long)address);    
    }
    
    return address;
 }

/* Finding the struct mem_chunk in mem_list with the given address */
struct mem_chunk *find_mem(void * address) {
   
    //Making a pointer to mem_list
    struct mem_chunk * current = mem_list;

    while(current != NULL){
        //Checking if current is the last node in this list
        if(current->address == address){
            break;
        }
        //Changing the current pointer to point to the next struct mem_chunk
        current = current->next;
    }
    //Returning struct mem_chunk
    return current;
}

/* Mark all addresses in mem_list as not in use */
void reset(){
    //making a pointer to traverse the mem_list
    struct mem_chunk * current = mem_list;

    //traversing to the end of the list, updating the initial size of mem_list and setting in_use to NOT_USED
    while(current != NULL){
        current->in_use = NOT_USED;
	initial_size++;
	current = current->next;
    }
}

/* Remove the memory not being used from the mem_list
*/
void sweep(){
    
    //making pointer to point to current, previous and the node to be removed
    struct mem_chunk* current = mem_list;
    struct mem_chunk* to_rem;
    struct mem_chunk* previous = mem_list;
 
    /* Handle the cases when list is empty or the address
     * is found at the front of the mem_list is not in use
    */
    if (mem_list == NULL){
        exit(0);
    } 

    //traversing to the end of the list
    while(current != NULL){
        //checking if current is not used
	if(current->in_use == NOT_USED){ 
	    //if current is pointing to the head of the global mem_list
	    if (current->address == mem_list->address){
		
		//updating mem_list and setting to_rem to the node to be removed
		mem_list = current->next;
		to_rem = current;
		
		//checking if debug option is set
		if (debug){
  	            printf("Sweep Handler 1: %lx\n", (unsigned long)to_rem->address);
		}
		//freeing the address and the mem_list node
        	free(to_rem->address);
        	free(to_rem);

		//updating current and preveious
		current = mem_list;
		previous = mem_list; 
	    }
	    //if current is pointing to the rest of the global mem_list
	    else{
		 //updating pointers and setting to_rem to the node to be removed
		to_rem = current;
		previous->next = previous->next->next;
		current = previous->next;

		if (debug){        	
		    printf("Sweep Handler 2: %lx\n", (unsigned long)to_rem->address);
		}
		//freeing the address and the mem_list node
        	free(to_rem->address);
        	free(to_rem);
	    }
	    //updating total number of deletions
	    totalDeletions++;
        }
        else{
	    previous = current;
	    current = current->next;
        }
    }
}

/* Executes the garbage collector.
 * mark_obj will traverse the data structure rooted at obj, calling
 * mark_one for each dynamically allocated chunk of memory to mark
 * that it is still in use.
 */
void mark_and_sweep(void *obj, void (*mark_obj)(void *)){
    
    //opening file to log entries
    FILE *log;
    log = fopen(LOGFILE, "a+");

    //reseting mem_list and updating log to with initial number of nodes in mem_list, , then sweeping
    reset();
    fprintf(log, "Number of memory chunks in mem_list before being freed: %d\n", initial_size);

    //marking used object as 1 and updating log to marked_one objects
    mark_obj(obj);
    fprintf(log, "Number of memory chunks in still being used: %d\n", marked_one);
    fprintf(log, "-------------------------------------------------------------\n");
    
    //calling sweep to free unused memory
    sweep();
    fprintf(log, "Number memory chunks freed: %d\n\n\n", totalDeletions);
    
    fclose(log);

    //reseting log variables
    initial_size = 0;
    marked_one = 0;
    totalDeletions = 0;

}

/* Mark vptr as still in use
 * Return code:
 *   0 on success
 *   1 if memory chunk pointed to by vptr was already marked as in use
 */
int mark_one(void *vptr){

    //find each mem address passed in by vptr in mem_list and mark as in_use
    struct mem_chunk * curr_mem;
    
    if (((curr_mem = find_mem(vptr)) == NULL) || curr_mem->in_use == USED){
	return 1;    
    }
    //setting curr_mem to USED and updating marked_one pointer
    curr_mem->in_use = USED;
    marked_one++;
    
    //debug statement
    if (debug){
        printf("In mark_one, %lxn as USED\n",(unsigned long) vptr);
    }

    return 0;
}

