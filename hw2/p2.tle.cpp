// O(N*M^2)
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

const int INF = 0x7f7f7f7f;

int N,M,K;
int A[100010];
int dp[100010][310];
// dp[i][j]: minimum accumulated cost to the status
// where one hand is on A[i], the other on j

inline int cost(int a, int b) {
    // cost when moving from a to b
    return max(abs(a - b) - K, 0);
}

int main() {
#ifdef soyccan
    freopen("p2.in", "r", stdin);
#endif
    while (scanf("%d%d%d", &N, &M, &K) != EOF) {
        FOR(i, 0, N) scanf("%d", &A[i]);

        memset(dp, INF, sizeof dp);
        FOR(j, 0, M) dp[0][j] = 0;
        FOR(i, 1, N)
            FOR(j, 0, M)
                FOR(k, 0, M) {
                    int mincost = min(cost(A[i-1], A[i]) + cost(k, j   ),
                                      cost(A[i-1], j   ) + cost(k, A[i]));
                    minimize(dp[i][j], dp[i-1][k] + mincost);
                }

        FOR(j, 0, M) {
            FOR(i, 0, N)
                LOG("%3d", dp[i][j]);
            LOG("\n");
        }

        printf("%d\n", *min_element(&dp[N-1][0], &dp[N-1][M]));
    }
    return 0;
}

