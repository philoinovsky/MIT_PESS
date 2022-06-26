- heap storage in C
    - allocation: `malloc`
    - aligned allocation: `memalign`
        1. align for cacheline
        2. align for vectorization
    - deallocation: `free`
        - fail to free
        - double freeing: segfault/free pointer assigned to others
- allocating virtual memory - `mmap()`
    - `mmap()` is lazy, it doesn't immediately allocate physical memory for the requested allocation
    - it populates the page table with entries pointing to a special zero page and marks the page read only
    - first write into a page causes a page fault, at that point, OS allocates a physical page, modifies the page table, and restarts the instruction
- mmap vs malloc
    - `malloc` is C library function, it may call `mmap` to allocate memory
    - `mmap` is system call
## Glossaries
- allocator speed
    - definition: allocator speed is the number of allocations and deallocations per second that the allocator can sustain
    - it is more important to maximize allocator speed for small blocks than large blocks. Because for allocating small blocks, a fixed overhead is more significant.
- fragmentation
    - U is the bytes in use, A is the bytes allocated, `F = A/U`
    - A grows monotonically for many allocators
    - Theorem: fragmentation for binned free lists is `Fv = O(lgU)`
- fragmentation glossary
    - space overhead: space used by the allocator for bookkeeping
    - internal fragmentation: waste due to allocating larger blocks than the user requests
    - external fragmentation: waste due to the inability to use storage because it is not contiguous
    - blowup: for a parallel allocator, the additional space beyond what a serial allocator would require.
## Parallel Allocator Strategies
- scalability
    - as the number of threads grow, the time to perform an allocation or deallocation should not increase, possible reasons:
        - lock contention
### I. global heap
- default C allocator
- all threads (processors) share a single heap
- accesses are mediated by a mutex (or lock-free synchronization) to preserve atomicity
- blowup = 1
- slow - acquiring a lock is like an L2-cache access
## II. local heap
- each thread allocates out of its own heap
- no locking is necessary
- suffers from `memory drift`: blocks allocated by one thread are freed on another => unbounded blowup.
## III. local ownership
- each object is labeled with its owner.
- freed objects are returned to the owner's heap
- good
    - fast allocation and freeing of local objects
    - resilience to false sharing
- bad: freeing remote objects requires synchronizations
- `blowup <= P`
### IV. The Hoard Allocator
- P local heaps
- 1 global heap
- memory is organized into large superblocks of size S
- only superblocks are moved between the local heaps and the global heap
- good: fast, scalable, bounded blowup, resilience to false sharing
#### Hoard's Blowup
- Lemma: the maximum storage allocated in global heap is at most maximum storage allocated in local heaps
- Theorem: let U be the user footprint for a program, and let A be Hoard's allocator footprint. We have `A <= O(U + SP)`, and hence the blowup is `A/U = 1 + O(SP/U)`.
- Proof: Analyze storage in local heaps. Recall that `ui >= min(ai - 2S, ai/2)`. First term: at most 2S unutilized storage per heap for a total of O(SP). Second term: allocated storage is at most twice the used storage for a total of O(U)
### V. Other solutions
- jemalloc: like Hoard with a few differences.
- SuperMalloc: up-and-coming contender