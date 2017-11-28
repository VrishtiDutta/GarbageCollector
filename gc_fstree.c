#include <stdio.h>
#include "gc_fstree.h"
#include "gc.h"

void mark_fstree(void * root){
    Fstree * curr_node = (Fstree *) root;
    Link * head = curr_node->links;
    if (mark_one(curr_node) != 1){
	mark_one(curr_node->name);
	while(head != NULL){
	    mark_one(head);
	    mark_fstree(head->fptr);
	    head = head->next;
        } 
    }
}

