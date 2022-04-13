5. what did I see
```
clang -O3 -DNDEBUG -Wall -std=c99 -D_POSIX_C_SOURCE=200809L -c testbed.c -o testbed.o
clang -O3 -DNDEBUG -Wall -std=c99 -D_POSIX_C_SOURCE=200809L -c matrix_multiply.c -o matrix_multiply.o
clang -o matrix_multiply testbed.o matrix_multiply.o -lrt -flto -fuse-ld=gold
```
6. what did I see when using asan
```
=================================================================
==4948==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 48 byte(s) in 3 object(s) allocated from:
    #0 0x494b2d in malloc (/home/philoy/MIT_PESS/Assignments/Homework1/matrix-multiply/matrix_multiply+0x494b2d)
    #1 0x4c4ed9 in make_matrix /home/philoy/MIT_PESS/Assignments/Homework1/matrix-multiply/matrix_multiply.c:39:24
    #2 0x7fdef74cf0b2 in __libc_start_main /build/glibc-sMfBJT/glibc-2.31/csu/../csu/libc-start.c:308:16

Indirect leak of 192 byte(s) in 12 object(s) allocated from:
    #0 0x494b2d in malloc (/home/philoy/MIT_PESS/Assignments/Homework1/matrix-multiply/matrix_multiply+0x494b2d)
    #1 0x4c4f67 in make_matrix /home/philoy/MIT_PESS/Assignments/Homework1/matrix-multiply/matrix_multiply.c:48:35

Indirect leak of 96 byte(s) in 3 object(s) allocated from:
    #0 0x494b2d in malloc (/home/philoy/MIT_PESS/Assignments/Homework1/matrix-multiply/matrix_multiply+0x494b2d)
    #1 0x4c4f20 in make_matrix /home/philoy/MIT_PESS/Assignments/Homework1/matrix-multiply/matrix_multiply.c:46:31
    #2 0x7fdef74cf0b2 in __libc_start_main /build/glibc-sMfBJT/glibc-2.31/csu/../csu/libc-start.c:308:16

SUMMARY: AddressSanitizer: 336 byte(s) leaked in 18 allocation(s).
```
7. after fixing the memory issue
```
Setup
Matrix A:
------------
    3      7      8      1
    7      9      8      3
    1      2      6      7
    9      8      1      9
------------
Matrix B:
------------
    1      3      0      1
    5      5      7      8
    0      1      9      8
    9      3      1      7
------------
Running matrix_multiply_run()...
---- RESULTS ----
Result:
------------
   47     55    122    130
   79     83    138    164
   74     40     75    114
  130     95     74    144
------------
---- END RESULTS ----
Elapsed execution time: 0.000000 sec
```
8. after freeing the matrix
```
==6838== Memcheck, a memory error detector
==6838== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==6838== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==6838== Command: ./matrix_multiply -p
==6838==
Setup
Matrix A:
------------
    3      7      8      1
    7      9      8      3
    1      2      6      7
    9      8      1      9
------------
Matrix B:
------------
    1      3      0      1
    5      5      7      8
    0      1      9      8
    9      3      1      7
------------
Running matrix_multiply_run()...
---- RESULTS ----
Result:
------------
   47     55    122    130
   79     83    138    164
   74     40     75    114
  130     95     74    144
------------
---- END RESULTS ----
Elapsed execution time: 0.000622 sec
==6838==
==6838== HEAP SUMMARY:
==6838==     in use at exit: 0 bytes in 0 blocks
==6838==   total heap usage: 39 allocs, 39 frees, 1,680 bytes allocated
==6838==
==6838== All heap blocks were freed -- no leaks are possible
==6838==
==6838== For lists of detected and suppressed errors, rerun with: -s
==6838== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
9. no aws account
10. no aws account