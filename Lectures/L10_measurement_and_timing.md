## Quiescing Systems
- DVFS
    - Dynamic voltage and frequency scaling
    - power ~= C * V^2 * f
        - C: capacity
        - V: voltage
        - f: clock frequency
- Measurement and Timing
    - Go after variance first, produce results reliably, after that, adjust the precision.
- Sources of Variability
    - Daemons and background jobs
    - Interrupts
    - Code and data alignment
    - Thread placement (don't measure on core 0)
    - Runtime scheduler
    - Hyperthreading
    - Multitenancy
    - DVFS
    - Turbo Boost
        - if there is only 1 job, it increases clock frequency to boost that job
    - Network traffic
- What you should do?
    - make sure no other jobs are running
    - shut down daemons and cron jobs
    - disconnect the network
    - don't fiddle with the mouse
    - for serial jobs, don't run on core 0, where intterrupt handlers are usually run
    - turn hyperthreading off
    - turn off DVFS
    - turn off Turbo Boost
    - use `taskset` to pin cilk workers to cores
    - etc., etc. (already done for you with `awsrun`)
- it is impossible to get deterministic results, what are the non-deterministic factors in hardware?
    - memory errors: alpha particle collided with one of the bits and flipped it. Hardware takes an extra cycle to correct it.
- Code Alignment
    - a small change to one place in the source code can cause much of the generated machine code to change locations. Performance can vary due to changes in cache alignment and page alignment
    - changing the order in which the *.o files appear on the linker command line can have a larger effect than going between -O2 and -O3
    - LLVM Alignment Switches
        - `-align-all-functions=<uint>`: force alignment of all functions
        - `-align-all-blocks=<uint>`: force alignment of all blocks in the function
        - `-align-all-nofallthru-blocks=<uint>`: force alignment of all blocks that have no fall-through predecessors (i.e. don't add nops that are executed)
    - program's name can affect its performance
## Tools for Measuring Software Performance
- five ways to measure
    - measure externally: `/usr/bin/time`
    - instrument the program:
        - `clock_gettime()`
        - `rdtsc()` clock cycles since boot
            - dont use it, may run backwards, may get different answers on different cores.
        - don't use `gettimeofday()`, it has similar problems
    - interrupt the program: `gdb` and `gprof`
    - exploit hardware and operating system support
        - `libpfm4` virtualizes all the hardware counters, can't get 4-5 numbers at the same time, otherwise you pay for performance/accuracy
        - `perf stat` employs it
    - simulate the program: `cachegrind`
## Performance Modelling
- Basic Workflow
    1. measure A
    2. change A to produce a hopefully faster A'
    3. measure A'
    4. if A' beats A, set A = A'
    5. if A is still not fast enough, go to 2
- when you looking at ratios, use geometrics mean
- least square regression
    - `T = a * I + b * C`
        - T: total time
        - I: number of instructions
        - C: number of cache misses
- Issues with Modelling
    - adding more basis functions to the model improves the fit, but how do I know whether I'm overfitting?
        - removing a basis function doesn't affect the quality much
    - is the model predictive?
        - train set and test set
    - how can I tell whether I'm fooling myself?
        - triangulate
        - check that different ways of measuring tell a consistent story