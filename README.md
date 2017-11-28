# GarbageCollector
Mock garbage collector that works on simulated file system (i.e. the two data stuctures: linked list and tree). The mark and 
sweep algorithm employed in this garbage collector works when gc_malloc (simulated "malloc" system call) is called to allocate 
memory for any data structure. This mock allocation of memory also "marks" the memory as in use, thereby allowing "mark_and_sweep"
to free memory based on what is not marked as in use.
