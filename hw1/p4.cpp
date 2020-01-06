// DP, edit distance
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <utility>
#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
#define minimize(var, val) var = min(var, val)
using namespace std;

const int INF = 1000000010;

int dp[2019][2019];
char A[2019], B[2019];

int main() {
#ifdef soyccan
    freopen("p4.in", "r", stdin);
#endif
    while (fgets(A, sizeof A, stdin) != NULL
            && fgets(B, sizeof B, stdin) != NULL) {

        int la = strlen(A) - 1;
        int lb = strlen(B) - 1;
        if (la > lb) {
            swap(A, B);
            swap(la, lb);
        }

        FOR(i, 0, la+1)
            dp[i][0] = i;
        FOR(j, 0, lb+1)
            dp[0][j] = j;
        dp[0][0] = 0;
        FOR(i, 1, la+1) {
            FOR(j, 1, lb+1) {
                dp[i][j] = min({
                        dp[i-1][j] + 1,
                        dp[i][j-1] + 1,
                        dp[i-1][j-1] + (A[i-1] == B[j-1])});
            }
        }
#ifdef soyccan
        FOR(i, 0, la+1) {
            FOR(j, 0, lb+1) {
                if (dp[i][j] == INF)
                    printf("%3d", -1);
                else
                    printf("%3d", dp[i][j]);
            }
            puts("");
        }
#endif
        int ans = dp[la][lb];
        {
            // for two strings of different length,
            // the insertinos and deletions at their tail can be omitted
            FOR(j, 1, lb+1)
                minimize(ans, dp[la][j]);
        }
        printf("%d\n", ans);
    }
    return 0;
}
