#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "gc.h"
#include "gc_list.h"

int main(int argc, char **argv) {
    List *ll = NULL;

    /*CASE 1: If there is only one node in the List*/
    printf("Adding node \n");
    add_node(ll, 15);
    printf("CASE 1: List is \n");
    char * str = to_str(ll);
    printf("%s\n", str);
    free(str);
    printf("Removing node \n");
    remove_node(ll, 15);
    mark_and_sweep(ll, mark_list);
    printf("Adding node \n");
    
    
    /*CASE 2: If there is more than one node in the List*/
    printf("Adding node \n");
    add_node(ll, 15);
    add_node(ll, 5);
    printf("CASE 2: List is \n");
    char * str = to_str(ll);
    printf("%s\n", str);
    free(str);
    printf("Removing node \n");
    remove_node(ll, 15);
    mark_and_sweep(ll, mark_list);

    return 0;
