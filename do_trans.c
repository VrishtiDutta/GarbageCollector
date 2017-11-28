/* Read and execute a list of operations on a linked list.
 * Periodically call the garbage collector.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "list.h"
#include "gc.h"
#include "gc_list.h"


#define MAX_LINE 128
#define ADD_NODE 1
#define DEL_NODE 2
#define PRINT_LIST 3


static List *ll = NULL;

void sig_handler(int signal){
    if (signal  == SIGUSR1){
	mark_and_sweep(ll, mark_list);
    }
}

int main(int argc, char **argv) {
    char line[MAX_LINE];
    char *str;
    
    //initializing signal set and adding SIGUSR1 to the set 
    sigset_t sig;
    if((sigemptyset(&sig) == -1) || (sigaddset(&sig, SIGUSR1) == -1)){
	perror("signal initialization failed");
	exit(1);
    }

    //initializing my signalhandler 
    void sig_handler(int sig);
    struct sigaction sh;
    sh.sa_handler = sig_handler;
    sh.sa_flags = 0;
    sigemptyset(&sh.sa_mask);
    if (sigaction(SIGUSR1, &sh, NULL) == -1){
	perror("SIGUSR1 handler initialization failed");
	exit(1);
    }

    if(argc != 2) {
        fprintf(stderr, "Usage: do_trans filename\n");
        exit(1);
    }

    FILE *fp;
    if((fp = fopen(argv[1], "r")) == NULL) {
        perror("fopen");
        exit(1);
    }

    int count = 1;

    //Installing the signal handler in the program 
    signal(SIGUSR1, sig_handler);

    while(fgets(line, MAX_LINE, fp) != NULL) {

        char *next;
        int value;
        int type = strtol(line, &next, 0);

     
     switch(type) {
            case ADD_NODE :
                if(sigprocmask(SIG_BLOCK, &sig, NULL) != 0) {
		    exit(1);
		}
		printf("SIGUSR1 blocked!\n");
		sleep(4);
		
                value = strtol(next, NULL, 0);
                ll = add_node(ll, value);

		printf("SIGUSR1 unblocked!\n");
		
		//restoring old signal mask
		if(sigprocmask(SIG_UNBLOCK, &sig, NULL) != 0) {
	 	    exit(1);
		}
                sleep(2);
		
                break;
            case DEL_NODE :
                value = strtol(next, NULL, 0);
                ll = remove_node(ll, value);
                break;
            case PRINT_LIST :
                str = tostring(ll);
                printf("List is %s\n", str);
		free(str);
                break;
            default :
                fprintf(stderr, "Error: bad transaction type\n");

        }
    
        
      if(count % 1 == 0) {
	    printf("calling mark_and_sweep\n");
            mark_and_sweep(ll, mark_list);
            // You might want to add something here to
            // make your program pause long enough to see what it
            // is doing. In the commented out code, we wait for 
            // any user input before continuing.
 
            //char check[MAX_LINE];
            //quitfgets(check, MAX_LINE, stdin);
       }       
        count++;

    }
    return 0;
}
