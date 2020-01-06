// O(N*M)
// DP monotonic queue optimization
#include <stdio.h>
#include <string.h>
#define FOR(i, a, n) for (int i = a; i < n; ++i)
#ifdef soyccan
#define LOG(format, ...) { fprintf(stderr, format, ##__VA_ARGS__); fflush(stdout); }
#else
#define LOG(...)
#endif
#define minimize(var, val) _minimize(&var, val)

const int INF = 0x7f7f7f7f;

int N,M,K;
int A[100032];

// dp[i][j]: minimum accumulated cost to the status
// where one hand is on A[i], the other on j
int dp[100032][320];

// tmp[x]: minimum accumulated cost to one hand on A[i+1], the other on x
int tmp[320];
// prefix min and suffix min for tmp
int pmn[320];
int smn[320];

// ascending queue, store indices of tmp[j-K : j+K]
int q[320];

void _minimize(int* var, int val) {
    if (val < *var) *var = val;
}

int min(int x, int y) {
    return x < y ? x : y;
}

int cost(int a, int b) {
    // cost when moving from a to b
    int t = a - b;
    if (t < 0) t = -t;
    return t <= K ? 0 : t - K;
}

int main() {
#ifdef soyccan
    freopen("p2.in", "r", stdin);
#endif
    while (scanf("%d%d%d", &N, &M, &K) != EOF) {
        FOR(i, 0, N) scanf("%d", &A[i]);

        memset(dp, INF, sizeof dp);
        FOR(i, 0, N) {
            // step 1: attain dp[i][j] with information in tmp
            int qh = 0, qt = 0; // head and tail of queue
            FOR(x, 0, K) {
                while (qt > qh && tmp[q[qt-1]] >= tmp[x])
                    --qt;
                q[qt++] = x;
            }
            FOR(j, 0, M) {
                if (i == 0) {
                    dp[i][j] = 0;
                    continue;
                }

                if (j + K < M) {
                    while (qt > qh && tmp[q[qt-1]] >= tmp[j+K])
                        --qt;
                    q[qt++] = j + K;
                }
                if (q[qh] < j - K)
                    qh++;

                minimize(dp[i][j], tmp[q[qh]]);
                if (j - K - 1 >= 0) minimize(dp[i][j], pmn[j-K-1] + j - K);
                if (j + K + 1 <  M) minimize(dp[i][j], smn[j+K+1] - j - K);
            }

            if (i == N-1) break;

            // step 2: generate tmp for next row's use
            memset(tmp, INF, sizeof tmp);

            FOR(j, 0, M) {
                minimize(tmp[j], dp[i][j] + cost(A[i], A[i+1]));
                minimize(tmp[A[i]], dp[i][j] + cost(j, A[i+1]));
            }

            pmn[0] = tmp[0] - 0;
            FOR(x, 1, M)
                pmn[x] = min(pmn[x-1], tmp[x] - x);

            smn[M-1] = tmp[M-1] + M-1;
            for (int x = M-2; x >= 0; x--)
                smn[x] = min(smn[x+1], tmp[x] + x);
        }

#ifdef soyccan
        FOR(j, 0, M) {
            FOR(i, 0, N)
                LOG("%3d", dp[i][j]);
            LOG("\n");
        }
#endif

        int ans = INF;
        FOR(j, 0, M)
            minimize(ans, dp[N-1][j]);
        printf("%d\n", ans);
    }
    return 0;
}
