- Compiler Reports
    - compier produce reports for many of its transformation passes
    - `-Rpass=<string>`: reports successful <string> optimization
    - `-Rpass-missed=<string>`: reports failed optimization
    - `-Rpass-analysis=<string>`
## Example Compiler Optimizations
### A. Data Structures
- None of those things
- Register allocation
- Memory to registers
- Scalar replacement of aggregates
- Alignment
### B. Loops
- Vectorizing
- Unswitching
- Idiom replacement
- Loop fission
- Loop skewing
- Loop tiling
- Loop interchange
### C. Logic
### D. Functions
- Unswitching
- Argument elimination
## --
### A. Optimizing a scaler
- O0
    - allocate storage
    - store `a` onto the stack
    - load `a` from the stack
- O1
    - replace loaded values with original register
    - remove dead code
### B. Optimizing a Structure
- O0
    - allocate stack storage for a struct
    - store the first field
    - store the second field
    - load the first field
    - load the second field
### C. Optimizing Function Calls
- Inlining
    1. copy code from `vec_scale`
    2. remove call and return
- Why not inlining all functions
    - recursive call are impossible to inline
    - compiler cannot inline a function defined in another compilation unit unless one uses whole-program optimization
    - function inlining can increase code size
- How does the compiler know whether or not inlining?
    - it guess based on heuristics
    - mark function with `__attribute__((always_inline))` or `__attribute__((no_inline))`
    - use link-time optimization (LTO) to enable whole-program optimization
### D. Loop Optimizations
- code motion, if you have two loops, you move the invariants of inner loop to outer loop
## What compilers cannot do
- compiler is unlikely to exploit symmertry