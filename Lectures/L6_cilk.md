## I. cache coherence - MSI protocol
- each cacheline has a state
    - M: modified, S: sharing, I: invalid
    - cacheline: 64B/128B
- when you load, you check if it is in M or S state, if I, you fetch from other processor's cache or from main memory
- apart from MSI, we have MESI and MOESI
- invalidation storm: when many cores are trying to modify the same value
## II. concurrency platforms
### A. Pthreads and WinAPI threads
- introduction
    - each thread implements an abstraction of a processor
    - threads communicate through shared memory
    - library functions mask the protocols involved in interthread coordination
    - `pthread_create` and `pthread_join`
- issues
    - overhead: cost of thread creation > 10^4 cycles => coarse-grained concurrency
    - scalability: need to rewrite for more cores
    - modularity: Fibonacci logic is no longer neatly encapsulated in the fib() function, we have logic in the both fib() implementation and caller function. 
    - code simplicity: must marshal arguments and engage in error-prone protocols in order to load-balance
### B. Thread Building Block (Intel)
- Introduction
    - specifies `tasks` rather than threads
    - load balanced using a `work-stealing` algo
    - focus on performance
- Other TBB Features
    - many C++ templates
        - `parallel_for`, `parallel_reduce`, `pipeline`, `filter`
        - provides concurrent container classes (thread-safe)
        - provides a variety of mutual-exclution library functions, including locks and atomic updates
### C. OpenMP
- Introduction
    - several compilers available (GCC, ICC, Clang, Visual Studio)
    - Linguistic extensions to C/C++ and Fortran in the form of compiler pragmas
    - supports loop parallelism, task parallelism and pipeline parallelism
- Other OpenMP Features
    - many pragma directives
        - `parallel for`: loop parallelism
        - `reduction`: data aggregation
        - directives for scheduling and data sharing
    - supplies a variety of `synchronization constructs`, such as barriers, atomic updates and mutex locks
### D. Cilk
- Introduction to Intel Cilk Plus
    - Cilk part is a small set of linguistic extentions to C/C++ to support fork-join parallelism. (The Plus part supports vector parallelism)
    - Features a provably efficient `work-stealing` scheduler.
    - Provides a `hyperobject` library for parallelizing code with global variables.
    - Ecosystem: `Cilkscreen` race detector and `Cilkview` scalability analyzer
    - use Tapir/LLVM compiler
- Reducer in Cilk
    - use reducer when (add, multiply, min, max, and, or, xor, etc.)
    - reducers can be created for `monoids` (associative binary operation)
- Serial Semantics
    - the serial elision of a cilk program is always a legal interpretation of the program's semantics
    - cilk keywords `grant permissions` for parallel execution. They do not `command` parallel execution.
    - serial testing: 1. use serial elision; 2. run on one
- Scheduling
    - theoretically efficient, but TBB and OpenMP are not.
    - dynamically scheduling