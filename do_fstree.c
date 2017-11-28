#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "gc_fstree.h"
#include "gc.h"

extern Fstree *root;

void sig_handler(int signal){
    if (signal  == SIGUSR1){
	mark_and_sweep(root, mark_fstree);
    }
}

int main() {
    
    //Installing the signal handler in the program 
    signal(SIGUSR1, sig_handler);

    //initializing my signalhandler 
    void sig_handler(int sig);
    struct sigaction sh;
    sh.sa_handler = sig_handler;
    sh.sa_flags = 0;
    sigemptyset(&sh.sa_mask);
    if (sigaction(SIGUSR1, &sh, NULL) == -1){
	perror("SIGUSR1 handler initialization failed");
	return 1;
    }

  
    root = create_node("root");

    //do_transactions("fs_trans1");
    //print_tree(root, 0);

    /* try uncommenting various bits to use other test files */

    do_transactions("fs_trans2");
    print_tree(root, 0);

    //do_transactions("fs_trans_loop");
    // don't call print_tree once you have a loop. It isn't sophisticated 
    // enough to handle the loop

/*
    Or you can put the calls directly in here but be careful, you
    can't use string literals for the paths or you will get Segmentation errors.

    add_node("","one");  // add_node (path, name)
    print_tree(root, 0);
    add_node("one","oneone");  // add_node (path, name)
    print_tree(root, 0);
    printf("************\n");

    char dest_path[20] = "one/oneone";
    char src_path[20] = "";
    char name[10] = "one";

    // this makes a link as a child of  one/oneone back to /one
    add_hard_link(dest_path, src_path, name);
    // DON"T call print_tree now since it doesn't handle loops and 
    //  will print infinitely
    // print_tree(root, 0);

*/

    // once you've implemented your garbage collector on fstree, you can
    // call it from here at various points to clean up the garbage
    mark_and_sweep(root, mark_fstree);

    return 0;
}
