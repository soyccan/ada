// divide & conquer
#define NDEBUG // disable assertions
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <utility>
#include <map>
#include <vector>
#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
#define minimize(var, val) var = min(var, val)
#ifdef soyccan
#define LOG(format, ...) fprintf(stderr, format, ##__VA_ARGS__); fflush(stdout)
#else
#define LOG(...)
#endif
#define NDEBUG
using namespace std;

const int MAXN = 1000005;
int N;
int A[512345];

int pool[2012345];
vector<long long> inpool;
// pool : `mi` value => count of occurrence

long long mi[512345];
// condition to form continous segment: max - min = r - l
// Case 1: min | max
// => min - l = max - r
// mi[0:m] : min - idx
// mi[m+1:N] : max - idx
//
// Case 2: max | min
// => max - r = min - l
// mi[0:m] : max - idx
// mi[m+1:N] : min - idx
//
// Case 3: min max |
// => max - min + l = r
// mi[0:m] : max - min + idx
//
// Case 4: | min max
// => l = r + min - max
// mi[m+1:N] : idx + min - max
//

void pool_add(int x) {
    assert(x + MAXN >= 0);
    pool[x + MAXN]++;
    inpool.push_back(x);
}

void pool_del(int x) {
    assert(x + MAXN >= 0);
    pool[x + MAXN]--;
}

int pool_get(int x) {
    assert(x + MAXN >= 0);
    return pool[x + MAXN];
}

void pool_clear() {
    for (int i : inpool) {
        assert(i + MAXN >= 0);
        pool[i + MAXN] = 0;
    }
    inpool.clear();
}

long long _solve_case1(int l, int m, int r) {
    long long res = 0;

    int px, pn;
    pool_clear();
    // pointer to max/min
    // pool of possible left ends when right end is A[i] are A[px:pn]

    int mx, mn; // min/max
    int lmn; // min/max in left half

    mn = N+1, mx = 0;
    for (int i = m; i >= l; i--) {
        minimize(mn, A[i]);
        mi[i] = mn - i;
    }
    FOR (i, m+1, r+1) {
        maximize(mx, A[i]);
        mi[i] = mx - i;
    }

    px = m, pn = m;
    mn = N+1, mx = 0, lmn = N+1;
    FOR (i, m+1, r+1) {
        maximize(mx, A[i]);
        minimize(mn, A[i]);

        while (px >= l && A[px] < mx) {
            pool_add(mi[px]);
            --px;
        }

        while (pn > px) {
            minimize(lmn, A[pn]);
            if (lmn < mn)
                break;
            pool_del(mi[pn]);
            --pn;
        }

        res += pool_get(mi[i]);

        LOG("  px=%d pn=%d i=%d res=%d\n", px, pn, i, res);
        LOG("  mi: ");
        FOR(i, l, r+1)
            LOG("%d ", mi[i]);
        LOG("\n");
        LOG("  A: ");
        FOR(i, l, r+1)
            LOG("%d ", A[i]);
        LOG("\n");
    }
    return res;
}

long long _solve_case3(int l, int m, int r) {
    pool_clear();
    long long res = 0;

    int lmx = 0, lmn = N+1;
    for (int i = m; i >= l; i--) {
        maximize(lmx, A[i]);
        minimize(lmn, A[i]);
        mi[i] = lmx - lmn + i;
        pool_add(mi[i]);
    }

    int mx = 0, mn = N+1;
    int p = m; // left half pointer
    lmx = 0, lmn = N+1;
    FOR(i, m+1, r+1) {
        maximize(mx, A[i]);
        minimize(mn, A[i]);
        while (p >= l) {
            maximize(lmx, A[p]);
            minimize(lmn, A[p]);
            if (lmx > mx && mn > lmn)
                break;
            pool_del(mi[p--]);
        }
        res += pool_get(i);

//         LOG("p%d i=%d res=%d\n", p, i, res);
//         LOG("mx=%d mn=%d lmx=%d lmn=%d\n", mx, mn, lmx, lmn);
//         LOG("\n");
//         LOG("A: ");
//         FOR(i, l, r+1)
//             LOG("%d ", A[i]);
//         LOG("\n\n");
    }
    return res;
}

long long solve(int l, int r) {
    if (l == r) return 1;

    int m = (l + r) / 2;

    LOG("l=%d r=%d\n", l, r);

    long long res = 0;

    // Case 1: min | max
    res += _solve_case1(l, m, r);
    LOG("case1 res=%d\n", res);

    // Case 2: max | min
    reverse(A+l, A+r+1);
    res += _solve_case1(l, m - ((r - l) % 2 == 0), r);
    reverse(A+l, A+r+1);
    LOG("case2 res=%d\n", res);

    // Case 3: min max |
    res += _solve_case3(l, m, r);
    LOG("case3 res=%d\n", res);

    // Case 4: | min max
    reverse(A+l, A+r+1);
    res += _solve_case3(l, m - ((r - l) % 2 == 0), r);
    reverse(A+l, A+r+1);
    LOG("case4 res=%d\n", res);

    return solve(l, m) + solve(m+1, r) + res;
}

int main() {
#ifdef soyccan
    freopen("p3.in", "r", stdin);
#endif
    while (scanf("%d", &N) != EOF) {
        FOR (i, 0, N)
            scanf("%d", &A[i]);
        printf("%lld\n", solve(0, N-1));
    }
    return 0;
}
