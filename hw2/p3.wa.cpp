// O(N^3)
// wrong answer
// DP
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <utility>
#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
#define minimize(var, val) var = min(var, val)
#ifdef soyccan
#define LOG(format, ...) fprintf(stderr, format, ##__VA_ARGS__); fflush(stdout)
#else
#define LOG(...)
#endif
using namespace std;

int N,K;
int A[200];
int B[200];
int S[200][200]; // sum of B
int dp[200][200];
// dp[i][j] : max when choose j items from item 1 to i
// the i-th item must be chosen

int calc() {
    static int E[200];
    int res = 0;
    E[0] = B[0] = 0;
    FOR(i, 1, N+1)
        E[i] = E[i-1] + B[i-1],
        res += A[i] * E[i];

    LOG("A: ");
    FOR(i, 1, N+1) LOG("%3d", A[i]);
    LOG("\nB: ");
    FOR(i, 1, N+1) LOG("%3d", B[i]);
    LOG("\nE: ");
    FOR(i, 1, N+1) LOG("%3d", E[i]);
    LOG("\nres=%d\n\n", res);

    return res;
}

int main() {
#ifdef soyccan
//     freopen("p3.1.in", "r", stdin);
#endif
    while (scanf("%d%d", &N, &K) == 2) {
        FOR(i, 1, N+1) scanf("%d%d", &A[i], &B[i]);

        LOG("before:\n");
        calc();

        // bubble sort
        FOR(i, 0, N)
            FOR(j, 1, N)
                if (A[j+1] * B[j] < A[j] * B[j+1]
                        || (A[j+1] * B[j] == A[j] * B[j+1] && B[j] < B[j+1]))
                    // IMPORTANT: consider the case when equal, let B be larger-first
                    swap(A[j], A[j+1]),
                    swap(B[j], B[j+1]);

        LOG("after:\n");
        calc();

        FOR(i, 1, N+1) S[i][1]  = B[i];
        FOR(i, 1, N+1)
            FOR(j, 1, N+1)
                FOR(k, 1, i) {
                    int v = dp[k][j-1] + S[k][j-1] * A[i];
                    int w = S[k][j-1] + B[i];
                    if (v > dp[i][j]) {
                        dp[i][j] = v;
                        S[i][j] = w;
                    }
                    else if (v == dp[i][j] && w > S[i][j]) {
                        // NOTE this case
                        S[i][j] = w;
                    }
                }

        LOG("dp:\n");
        FOR(i, 1, N+1) {
            FOR(j, 1, N+1)
                LOG("%5d", dp[i][j]);
            LOG("\n");
        }
        LOG("S:\n");
        FOR(i, 1, N+1) {
            FOR(j, 1, N+1)
                LOG("%5d", S[i][j]);
            LOG("\n");
        }

        printf("%d\n", dp[N][K]);
    }
    return 0;
}


/*
n=9 k=4
i : 0  1  2  3  4  5  6 7  8
A : 6  0 14 14 14 13  3 5  3
B : 8 13 14  1  4  5 11 6 12
994

n=6 k=6
i :  0 1 2  3 4 5
A : 10 0 5 10 2 1
B :  7 8 3  7 9 3
673
*/
