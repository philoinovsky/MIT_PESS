- The Master Method
    - solving divide-and-conquer recurrences applies to recurrences of the form `T(n) = a * T(n/b) + f(n)`, where a >= 1, b >= 1, and f is asymptotically positive
        - case 1: increasing, n^logb(a) >> f(n), `T(N) = Θ(n^logb(a))`
        - case 2: same, n^logb(a) * lg(n)^k ~= f(n) for some k (k >= 0), then `T(N) = Θ(n^logb(a) * lg(n) ^ (k+1))`
        - case 3: decreasing, n^logb(a) << f(n), `T(N) = Θ(f(n))`
    - more general method: Arka-Bazzi Method
- `#pragma cilk grainsize G`
    - coarsening to avoid too much parallelism
    - parallelism is a threshold thing, if it is bigger than number of cores, you are fine.
- Three Performance Tips
    1. minimize the span to maximize parallelism.
    2. if you have plenty of parallelism, try to trade some of it off to reduce work overhead.
    3. use divide-and-conquer recursion or parallel loops rather than spawning one small thing after another
    4. ensure that work/#spawns is sufficiently large
    5. parallelize outer loops, as apposed to inner loops, if you're forced to make a choice
    6. watch out for scheduling overheads