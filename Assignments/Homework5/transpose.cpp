#include <bits/stdc++.h>
using namespace std;

// work: N^2
// span: 1/3*log4(N) + 256
// parallelism = N^2 / log4(N)
void transpose(vector<vector<int>>& A,
                size_t r1, size_t r2, size_t c1, size_t c2)
{
    if (c1 >= A.size() / 2 && r2 >= A.size() / 2)
        // if in the upper right corner, dont exchange at all
        return;
    if (r2 - r1 < 16 && c2 - c1 < 16)
        for (size_t i = r1; i < r2; i++)
            for (size_t j = c1; j < i; j++)
                swap(A[i][j], A[A.size()-i-1][A.size()-j-1]);
    // divide and conquer
    /* cilk_spawn */ transpose(A, r1, (r1 + r2)/2, c1, (c1 + c2)/2);
    /* cilk_spawn */ transpose(A, r1, (r1 + r2)/2, (c1 + c2)/2, c2);
    /* cilk_spawn */ transpose(A, (r1 + r2)/2, r2, c1, (c1 + c2)/2);
    transpose(A, (r1 + r2)/2, r2, (c1 + c2)/2, c2);
    /* cilk_sync */
}

int main()
{
    vector<vector<int>> A(500, vector<int>(500));
    transpose(A, 0, 500, 0, 500);
    return 0;
}