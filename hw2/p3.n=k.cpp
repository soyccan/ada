// accepted for testcases 0 (N = K)
// simple sort only
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

const int INF = 1000000010;

int N,K;
int A[200];
int B[200];
int E[200];

int main() {
#ifdef soyccan
    freopen("p3.in", "r", stdin);
#endif
    while (scanf("%d%d", &N, &K) == 2) {
        FOR(i, 1, N+1) scanf("%d%d", &A[i], &B[i]);
        int res = 0;

        E[0] = B[0] = 0;
        FOR(i, 1, N+1)
            E[i] = E[i-1] + B[i-1],
            res += A[i] * E[i];
        LOG("before:\nA: ");
        FOR(i, 1, N+1) LOG("%3d", A[i]);
        LOG("\nB: ");
        FOR(i, 1, N+1) LOG("%3d", B[i]);
        LOG("\nE: ");
        FOR(i, 1, N+1) LOG("%3d", E[i]);
        LOG("\nres=%d\n\n", res);

        for (int i = N-1; i >= 1; i--)
            FOR(j, 1, i+1)
                if (A[j+1] * B[j] < A[j] * B[j+1]
                        || (A[j+1] * B[j] == A[j] * B[j+1] && B[j] < B[j+1]))
                    swap(A[j], A[j+1]),
                    swap(B[j], B[j+1]);

        res = E[0] = B[0] = 0;
        FOR(i, 1, N+1)
            E[i] = E[i-1] + B[i-1],
            res += A[i] * E[i];
        LOG("after:\nA: ");
        FOR(i, 1, N+1) LOG("%3d", A[i]);
        LOG("\nB: ");
        FOR(i, 1, N+1) LOG("%3d", B[i]);
        LOG("\nE: ");
        FOR(i, 1, N+1) LOG("%3d", E[i]);
        LOG("\nres=%d\n\n", res);

        printf("%d\n", res);
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

