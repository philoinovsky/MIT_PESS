# Checkoff Items
1. bottleneck: `merge_a`
2. 
# Write-Ups
1. compare the cachegrind result of unoptimized code vs optimized code
    - optimized code
        - less ref, same cache miss
        - less branch predictions with a higher rate of success
2. inline `merge_i` and `copy_i`, performace
    - less ref to both instructions and data
3. downside of inlining recursive functions
    - reduce the hit rate of instruction cache (if it unrolls too much)
    - may result in a too large binary file
    - how can you inline `sort_i`?
4. substitute use of arrays with use of pointers, what makes pointers faster?
    - why pointers faster than arrays??? They generate the same assembly!
5. coarsing, how does performance change?
    - data reference reduced significantly (-25%), instruction reference reduced significantly (-25%)
6. reduce memory usage, explain perf difference?
    - much less data ref (-30%) and inst ref (-50%)
7. initialize a piece of memory in the beginning, release at the end. Explain perf difference?
    - less data ref and inst ref.