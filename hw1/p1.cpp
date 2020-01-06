// divide & conquer
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <utility>
#define int long long
#define FAKULONGLONG 878787
#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
#define minimize(var, val) var = min(var, val)
using namespace std;

int N;
int ans;
int p[1123456];
int f[1123456];


void mergesort(int l, int r) {
    if (l == r) return;
    int m = (l + r) / 2;

    mergesort(l, m);

    FOR(i, m+1, r+1) {
        ans += upper_bound(p+l, p+m+1, p[i]+f[i])
                - lower_bound(p+l, p+m+1, p[i]-f[i]);
    }

    mergesort(m+1, r);

    static int tmp[1123456];
    int lp=l, rp=m+1, len=0;
    while (lp <= m || rp <= r) {
        if (rp > r || (lp <= m && p[lp] < p[rp]))
            tmp[len++] = p[lp++];
        else
            tmp[len++] = p[rp++];
    }
    memcpy(p+l, tmp, sizeof(int) * len);
}

int32_t main() {
#ifdef soyccan
    freopen("p1.in", "r", stdin);
#endif

    while (scanf("%lld", &N) != EOF) {
        for (int i = 0; i < N; i++)
            scanf("%lld", &p[i]);
        for (int i = 0; i < N; i++)
            scanf("%lld", &f[i]);

        ans = 0;
        mergesort(0, N-1);
        printf("%lld\n", ans);
    }

    return 0;
}
