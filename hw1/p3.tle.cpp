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

int N;
int A[512345];

int main() {
#ifdef soyccan
    freopen("p3.in", "r", stdin);
#endif
    while (scanf("%d", &N) != EOF) {
        int ans = 0;
        FOR(i, 0, N) {
            scanf("%d", &A[i]);
            int mx = 0, mn = N+1;
            for (int j = i; j >= 0; j--) {
                maximize(mx, A[j]);
                minimize(mn, A[j]);
                if (i - j == mx - mn)
                    ans++;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
