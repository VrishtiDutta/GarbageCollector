#include <stdlib.h>
#include <stdio.h>
#include "gc_list.h"
#include "list.h"
#include "gc.h"

/* Marks the address of the nodes of the given List head as in use in the mem_list */
void mark_list(void * head){
    List * current;
    current = (List*)head;
    while(current != NULL){
       mark_one(current);
       current = current->next;
    }
}


