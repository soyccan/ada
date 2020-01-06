// O(N^2)
// TLE, correctness unknown
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
int A[212345];

int main() {
#ifdef soyccan
//     freopen("p4.in", "r", stdin);
#endif
    while (scanf("%d", &N) == 1) {
        int s[3];
        FOR(i, 0, 3) scanf("%d", &s[i]);
        sort(s, s+3);

        FOR(i, 0, N) scanf("%d", &A[i]);
        sort(A, A+N);

        int l = 0, r = N-1, ans = 0;
        while (l <= r) {
            ans++;
            int a = s[0], b = s[1], c = s[2];
            int _r = r;

            // beat strongest
            while (a || b || c) {
                if (c >= A[r]) c = 0, --r;
                if (b >= A[r]) b = 0, --r;
                if (a >= A[r]) a = 0, --r;

                for (int i = r; i > l; i--)
                    if (c >= A[i] || b >= A[i] || a >= A[i]) {
                        if (c >= A[i]) c = 0;
                        else if (b >= A[i]) b = 0;
                        else if (a >= A[i]) a = 0;

                        for (int j = i; j < r; j++)
                            swap(A[j], A[j+1]);
                        --r;
                    }

                if (a+b >= A[r]) a = b = 0, --r;
                if (a+c >= A[r]) a = c = 0, --r;
                if (b+c >= A[r]) b = c = 0, --r;
                if (a+b+c >= A[r]) a = b = c = 0, --r;

                break;
            }

            // if the strongest cannot be beat, the battle is lost
            if (_r == r) {
                ans = -1;
                break;
            }

            // beat weakest
            while (a || b || c) {
                if (a >= A[l]) a = 0, ++l;
                if (b >= A[l]) b = 0, ++l;
                if (c >= A[l]) c = 0, ++l;
                if (a+b >= A[l]) a = b = 0, ++l;
                if (a+c >= A[l]) a = c = 0, ++l;
                if (b+c >= A[l]) b = c = 0, ++l;

                break;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
