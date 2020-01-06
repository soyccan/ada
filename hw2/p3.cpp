// O(N^2 * sum(B))
// DP
#include <stdio.h>
#define FOR(i, a, n) for (int i = a; i < n; i++)
#ifdef soyccan
#define LOG(format, ...) fprintf(stderr, format, ##__VA_ARGS__); fflush(stdout)
#else
#define LOG(...)
#endif
#define maximize(var, val) _maximize(&var, val)
#define swap(x, y) _swap(&x, &y)

int N,K;
int A[128];
int B[128];
int dp[128][10048];
// dp[i][j][k] : max value if pick j items from item 1 to i
// and the sum of B of those j items equal k
// rolling => dp[j][k]

void _maximize(int* var, int val) {
    if (val > *var) *var = val;
}

void _swap(int* x, int* y) {
    int t = *x;
    *x = *y;
    *y = t;
}

int main() {
#ifdef soyccan
//     freopen("p3.in", "r", stdin);
#endif
    while (scanf("%d%d", &N, &K) == 2) {
        FOR(i, 1, N+1) scanf("%d%d", &A[i], &B[i]);

        int s = 0;
        FOR(i, 1, N+1) s += B[i];

        // bubble sort
        FOR(i, 0, N)
            FOR(j, 1, N)
                if (A[j+1] * B[j] < A[j] * B[j+1]
                        || (A[j+1] * B[j] == A[j] * B[j+1] && B[j] < B[j+1]))
                    // IMPORTANT: consider the case when equal, let B be larger-first
                    swap(A[j], A[j+1]),
                    swap(B[j], B[j+1]);

        FOR(i, 0, N+1)
            for (int j = K; j >= 0; j--)
                for (int k = s; k >= 0; k--) {
                    if (i == 0)
                        dp[j][k] = -1; // negative for invalid state

                    else if (i == 1 && j == 0 && k == 0)
                        dp[j][k] = 0;

                    else if (i == 1 && j == 1 && k == B[1])
                        dp[j][k] = 0;

                    else if (i > 1 && j <= i) {
                        if (j >= 1 && k >= B[i] && dp[j-1][k-B[i]] != -1)
                            maximize(dp[j][k], dp[j-1][k-B[i]] + A[i] * (k-B[i]));
                    }

                    else
                        dp[j][k] = -1;
                }

#ifdef soyccan
        FOR(i, 0, N+1) {
            LOG("i=%d\n", i);
            FOR(j, 0, K+1) {
                FOR(k, 0, 10)
                    LOG("%4d", dp[j][k]);
                LOG("\n");
            }
            LOG("\n\n");
        }
#endif
        int ans = -1;
        FOR(k, 0, s+1)
            maximize(ans, dp[K][k]);
        printf("%d\n", ans);
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

