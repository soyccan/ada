// O(N log N)
// greedy
#include <cstdio>
#include <algorithm>
#include <functional>
#include <set>
#define FOR(i, a, n) for (int i = a; i < n; i++)
#ifdef soyccan
#define LOG(format, ...) fprintf(stderr, format, ##__VA_ARGS__); fflush(stdout)
#else
#define LOG(...)
#endif
using namespace std;

const int INF = 1000000000;

int N;
multiset<int,greater<int>> A;

long long calc_score(int a[],
                     int an,
                     decltype(A)::iterator d[],
                     int* const dn) {
    // get priority value of given attack set 'a'
    // beaten ships will be placed in array 'd' (die)

    LOG("clrdie\n");
    long long r = 0; // total waste attack, <= 2e13
    *dn = 0;

    // the strongest must be beaten
    sort(a, a+an);
    if (a[an-1] < *A.begin())
        return -INF;
    LOG("beat %d with %d -> SUCCESS\n", *A.begin(), a[an-1]);
    d[(*dn)++] = A.begin();
    r += a[an-1] - *A.begin();

    FOR(i, 0, an-1) {
        decltype(A)::iterator p = A.lower_bound(a[i]);

        while (p != A.end() && count(d, d+*dn, p))
            ++p;

        if (p == A.end())
            return -INF;

        LOG("beat %d with %d -> SUCCESS\n", *p, a[i]);
        d[(*dn)++] = p;
        r += a[i] - *p;
    }
    return (long long)(3e13) * an - r;
}

int main() {
#ifdef soyccan
//     freopen("p4.in", "r", stdin);
#endif
    while (scanf("%d", &N) == 1) {
        int s[3];
        FOR(i, 0, 3) scanf("%d", &s[i]);

        A.clear();
        FOR(i, 0, N) {
            int x;
            scanf("%d", &x);
            A.insert(x);
        }

        int ans = 0;
        while (A.size()) {
            ans++;
            LOG("ans=%d\n", ans);

            // store dead ships
            decltype(A)::iterator die[3];
            int dien = 0;

            // attack sets
            int a = s[0], b = s[1], c = s[2];
            int atk[5][4] = {
                {3, a    , b, c},
                {2, a+b  , c   },
                {2, a+c  , b   },
                {2, b+c  , a   },
                {1, a+b+c      }
            };

            int target = -1;
            long long mxscore = -INF;
            FOR(i, 0, 5) {
                long long g = calc_score(atk[i]+1, atk[i][0], die, &dien);
                if (g > mxscore)
                    target = i,
                    mxscore = g;
            }

            if (target != -1) {
                calc_score(atk[target]+1, atk[target][0], die, &dien);
                FOR(i, 0, dien)
                    A.erase(die[i]);
            }
            else {
                ans = -1;
                break;
            }
        }

        printf("%d\n", ans);
    }
    return 0;
}
