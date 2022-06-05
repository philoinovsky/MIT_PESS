# Homework 3
## To Sum Up
- vectorized code
- to avoid pointers overlapping check?
    - use `restrict` keyword
- alignment check
    - use `__builtin_assume_aligned`
- flags to enable and debug vectorization
    - enable: `-fvectorize`
    - disable: `-fno-vectorize`
    - identify vectorized loops: `-Rpass=loop-vectorize`
    - identify failed loops: `-Rpass-missed=loop-vectorize`
- debugging through assembly code inspection
    - `perf annotate -f`
- `#pragma Clang loop`: http://Clang.llvm.org/docs/LanguageExtensions.html#extensions-for-loop-hint-optimizations
    - `interleave(enabled)` indicates the out-of-order instructions emit
    - `vectorize(enabled)` force loop to vectorize
    - `vectorize_width(N)` indicates the size of the register to fit in N elements, if N * sizeof(element) is greater than 64B(avx512 register), it will unroll the loop
## Write-up 1
- Look at the assembly code above. The compiler has translated the code to set the start index at -2^16 and adds to it for each memory access. Why doesn’t it set the start index to 0 and use small positive offsets?
    - because it uses `%rax` to store the remaining bytes
## Write-up 2:
- This code is still not aligned when using AVX2 registers. Fix the code to make sure it uses aligned moves for the best performance
    - align to 32B
## Write-up 3:
- Provide a theory for why the compiler is generating dramatically different assembly
    - compiler is too stupid
    - compiler recognized it as a branch, rather than a max operation
## Write-up 4: 
- Inspect the assembly and determine why the assembly does not include instructions with vector registers. Do you think it would be faster if it did vectorize? Explain.
    - It didn't vectorize, instead it calls the `memcpy` directly
    - Probably it won't be faster with SIMD, because `memcpy` could already implemented all the tricks that you may have in your hand
## Write-up 5:
- Check the assembly and verify that it does in fact vectorize properly. Also what do you notice when you run the command `$ clang -O3 example4.c -o example4; ./example4` with and without the -ffast-math flag? Specifically, why do you a see a difference in the output.
    - with `-ffast-math`: 0x1.755cccec10aa3p+3
    - without `-ffast-math`: 0x1.755cccec10aa5p+3
## Write-up 6:
- What speedup does the vectorized code achieve over the unvectorized code?
    - 3 times (0.008s vs 0.0259s) (uint32)
- What additional speedup does using `-mavx2` give? You may wish to run this experiment several times and take median elapsed times; you can report answers to the nearest 100% (e.g., 2×, 3×, etc).
    - 1.6 times speedup (0.005s vs 0.008s)
- What can you infer about the bit width of the default vector registers on the awsrun machines?
    - speedup = bitwidth multiples * 0.75
- What about the bit width of the AVX2 vector registers? Hint: aside from speedup and the vectorization report, the most relevant information is that the data type for each array is uint32_t.
## Write-up 7:
- Compare the contents of loop.s when the VECTORIZE flag is set/not set.
- Which instruction (copy its text here) is responsible for the vector add operation?
    - `movdqa	2080(%rsp,%rax,2), %xmm0`
- Which instruction (copy its text here) is responsible for the vector add operation when you additionally pass AVX2=1? You can find an x86 instruction manual on LMOD. Look for MMX and SSE2 instructions, which are vector operations.
    - `vmovdqu	2768(%rsp), %ymm0`
- To make the assembly code more readable it may be a good idea to remove debug symbols from release builds by moving the `-g` and `-gdwarf-3` CFLAGS in your Makefile. It might also be a good idea to turn off loop unrolling with the `-fno-unroll-loops` flag while you study the assembly code.
## Write-up 8:
- Use the __OP__ macro to experiment with different operators in the data parallel loop. For some operations, you will get division by zero errors because we initialize array B to be full of zeros—fix this problem in any way you like. Do any versions of the loop not vectorize with `VECTORIZE=1 AVX2=1`? Study the assembly code for << with just `VECTORIZE=1` and explain how it differs from the AVX2 version.
    - when __OP__ is `/`, it uses reciprocal instruction to compute the divide.
    - when __OP__ is `<<`, can't understand, but the AVX2 instructions seems much more understandable
## Write-up 9:
- What is the new speedup for the vectorized code, over the unvectorized code, and for the AVX2 vectorized code, over the unvectorized code, when you change __TYPE__ to uint64_t, uint32_t, uint16_t and uint8_t? For each experiment, set __OP__ to + and do not change N.
    - proportional to the (vector size / type size)
## Write-up 10:
- You already determined that uint64_t yields the least performance improvement for vectorized codes (Section 3.1.4).
- Test a vector multiplication (i.e., __OP__ is *) using uint64_t arrays. What happens to the AVX2 vectorized code’s speedup relative to the unvectorized code (also using uint64_t and *)? What about when you set the data type width to be smaller — say uint8_t?
    - uint32_t:
        - unvectorized: 0.026s
        - SSE: 0.016s (60%)
        - AVX2: 0.006s (25%)
    - uint16_t:
        - unvectorized: 0.033s
        - SSE: 0.0044s (1/8)
        - AVX2: 0.000025s (1/1000)
    - uint8_t:
        - unvectorized: 0.033s
        - SSE: 0.000025s (1/1000)
        - AVX2: 0.000025s (1/1000)
## Write-up 11:
- Open up the aws-perf-report tool for the AVX2 vectorized multiply code using `uint64_t` (as you did in Recitation 2). Remember to first use the awsrun perf record tool to collect a performance report. Does the vector multiply take the most time? If not, where is time going instead?
    - yes, but `vmovdqu` also takes 45%
- Now change __OP__ back to +, rerun the experiment and inspect aws-perf-report again. How does the percentage of time taken by the AVX2 vector add instruction compare to the time spent on the AVX2 vector multiply instruction?
    - `vmovdqu` takes more than 50%
## Write-up 12:
- Get rid of the `#define N 1024 macro` and redefine N as: `int N = atoi(argv[1]);` (at the beginning of main()). (Setting N through the command line ensures that the compiler will make no assumptions about it.) Rerun (with various choices of N) and compare the AVX2 vectorized, non-AVX2 vectorized, and unvectorized codes. Does the speedup change dramatically relative to the N = 1024 case? Why?
    - no, the compiler will change the vectorized loop to the closest place (in SSE `N & ~0xf`, in AVX2 `N & ~0x1f`), the rest will be handled by the `addl` instruction.
## Write-up 13:
- Set __TYPE__ to `uint32_t` and __OP__ to `+`, and change your inner loop to be strided. Does clang vectorize the code? Why might it choose not to vectorize the code?
    - no, because it need takes extra operations to do so. it doesn't know how to add strided numbers faster
## Write-up 14:
- Use the `#vectorize` pragma described in the clang language extensions webpage above to make clang vectorize the strided loop.
- What is the speedup over non-vectorized code for non-AVX2 and AVX2 vectorization?
    - it becomes slower
- What happens if you change the vectorize_width to 2?
    - much slower
- Play around with the clang loop pragmas and report the best you found (that vectorizes the loop). Did you get a speedup over the non-vectorized code?
    - yes, we can do it when we enable vectorization, performance improved from 0.015s to 0.013s
## Write-up 15:
- This code vectorizes, but how does it vectorize? Turn on ASSEMBLE=1, look at the assembly dump, and explain what the compiler is doing.
```
	.loc	5 73 20 is_stmt 1       # loop.c:73:20
	vmovq	2080(%rsp,%rdx,2), %xmm0 # xmm0 = mem[0],zero
	.loc	5 73 32 is_stmt 0       # loop.c:73:32
	vmovq	1056(%rsp,%rdx,2), %xmm1 # xmm1 = mem[0],zero
	.loc	5 73 25                 # loop.c:73:25
	vpaddb	%xmm0, %xmm1, %xmm0
	.loc	5 73 18                 # loop.c:73:18
	vpextrb	$0, %xmm0, 32(%rsp,%rdx,2)
	vpextrb	$2, %xmm0, 34(%rsp,%rdx,2)
```