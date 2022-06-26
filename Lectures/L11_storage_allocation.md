## I. fixed size allocation
- free list
    - allocation from top of the list
    - free to top of the list
    - issue: external fragmentation. may have a lot page table entries, may be sparse, so the efficiency is bad; 
    - solution: free list per page, allocate from the fullest page
- bitmap: probably better
## II. variable-size heap allocation
- binned free lists: bin k holds memory blocks of size 2^k
- since a 64-bit address space is so huge, why not just allocate out of virtual memory and never free
    - external fragmentation would be horrendous. the performance of the page table would degrade tremendously leading to `disk thrashing`
- analysis of BFL:
    - suppose the maximum amount of heap memory in use is M, the amount of virtual memory consumed by heap storage is `O(MlgM)`
    - BFL Θ(1)
- coalescing
    - splicing together adjacent small blocks into a larger block
    - buddy system: can be used, but higher overhead.
## III. Garbage Collection
- terminology
    - roots: objects directly accessible by the program
    - live: reachable from the roots by following pointers
    - dead: objects are inaccessible and can be recycled.
    - how can GC identify pointers?
        - strong typing (C is not)
        - prohibit pointer arithmetic (which may slow down some programs)
- reference counting GC
    - can't deal with cyclic reference
- mark-and-sweep GC
    - objects and pointers form a directed graph `G = (V, E)`. Live objects are reachable from the roots. Use BFS to find the live objects
    - mark: BFS
    - sweep: free unmarked
    - issues:
        1. sweep all entire allocated objects
        2. doen't deal with fragmentation
- stop-and-copy GC
    - at a high level it is similar to mark-and-sweep.
    - use the queue for BFS as the new memory, so live object will be stored in new and contiguous memory.
## IV. Dynamic Storage Allocation
- lots more is known and unknown about dynamic storage allocation.
    - buddy system: coalescing
    - variants of mark-ans-sweep
    - generational GC
    - real-time GC
    - multithreaded storage allocation
    - parallel GC