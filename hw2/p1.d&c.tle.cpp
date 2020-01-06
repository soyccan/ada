// O(N^2)
// TLE, no sure if correct
// divide & conquer
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <utility>
#include <map>

#include <array>
#include <iostream>
#include <random>

#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
#define minimize(var, val) var = min(var, val)
#ifdef soyccan
#define LOG(format, ...) fprintf(stderr, format, ##__VA_ARGS__); fflush(stdout)
#else
#define LOG(...)
#endif
using namespace std;


namespace ada {

class Xoroshiro128 {
 public:
  using result_type = uint32_t;
  static constexpr result_type(min)() { return 0; }
  static constexpr result_type(max)() { return UINT32_MAX; }
  static inline result_type rotl(const result_type x, int k) {
    return (x << k) | (x >> (32 - k));
  }
  Xoroshiro128() : Xoroshiro128(1, 2, 3, 4) {}
  Xoroshiro128(result_type a, result_type b, result_type c, result_type d)
      : s{a, b, c, d} {}
  result_type operator()() {
    const result_type result = rotl(s[0] + s[3], 7) + s[0];
    const result_type t = s[1] << 9;
    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];
    s[2] ^= t;
    s[3] = rotl(s[3], 11);
    return result;
  }

 private:
  std::array<result_type, 4> s;
};

namespace {
    int c_lead, c_team;
    Xoroshiro128 rng;
}  // namespace

int Init() {
  int n;
  uint32_t s1, s2, s3, s4;
  std::cin >> n >> c_lead >> c_team >> s1 >> s2 >> s3 >> s4;
  rng = Xoroshiro128(s1, s2, s3, s4);
  return n;
}

int GetLeadership() { return uint64_t(rng()) * c_lead >> 32; }

int GetTeamValue() {
  int tmp = int(uint64_t(rng()) * c_team >> 32) + 1;
  return int(c_team / sqrt(tmp));
}

}  // namespace ada


/*******************************************************************************
 * Example usage:
 * const int kN = 2000000;
 * int leadership[kN], team_value[kN];
 * int main() {
 *   int n = ada::Init(); // Get N
 *   for (int i = 0; i < n; i++) leadership[i] = ada::GetLeadership();
 *   for (int i = 0; i < n; i++) team_value[i] = ada::GetTeamValue();
 * }
 ******************************************************************************/

struct Elem {
    int v; // value
    int i; // index
    Elem(int v = 0, int i = 0): v(v), i(i) {}
};

const int MOD = 1000000007;
const size_t NN = 2123456;
int N;
int L[NN]; // leadership
int T[NN]; // team awareness
Elem P[NN]; // (prefix sum of team awareness, index)
Elem LP[NN]; // (L+P, index)
vector<int> seg[NN]; // if a segment [l, r] forms a fleet: seg[r].push_back(l);
int dp[NN];

void sort_merge(Elem* arr, int l, int m, int r) {
    static Elem tmp[NN];
    int i = l, j = m+1, len = 0;
    while (i <= m || j <= r)
        if (j > r || (i <= m && arr[i].v < arr[j].v))
            tmp[len++] = arr[i++];
        else
            tmp[len++] = arr[j++];
    memcpy(arr+l, tmp, sizeof(Elem) * len);
}

void find_seg(int l, int r) {
    if (l == r) return;

    LOG("before: l=%d r=%d\n", l, r);
    LOG("P : ");
    FOR(i, 1, N+1) LOG("%3d", P[i].v);
    LOG("\nLP: ");
    FOR(i, 1, N+1) LOG("%3d", LP[i].v);
    LOG("\n\n");

    int m = (l + r) / 2;
    find_seg(l, m);
    find_seg(m+1, r);

    int pl = l;
    FOR(pr, m+1, r+1) {
        while (pl <= m && LP[pl].v < P[pr].v)
            pl++;
        FOR(i, pl, m+1) {
            LOG("[%3d, %3d]\n", LP[i].i, P[pr].i);
            seg[ P[pr].i ].push_back(LP[i].i);
        }
    }

    sort_merge(P, l, m, r);
    sort_merge(LP, l, m, r);

    LOG("after: l=%d r=%d\n", l, r);
    LOG("P : ");
    FOR(i, 1, N+1) LOG("(%3d, %3d) ", P[i].v, P[i].i);
    LOG("\nLP: ");
    FOR(i, 1, N+1) LOG("(%3d, %3d) ", LP[i].v, LP[i].i);
    LOG("\n\n");
}



int main() {
#ifdef soyccan
    freopen("p1.in", "r", stdin);
#endif
    N = ada::Init();
    FOR(i, 1, N+1) L[i] = ada::GetLeadership();
    FOR(i, 1, N+1) T[i] = ada::GetTeamValue();

    P[0].v = 0;
    FOR(i, 1, N+1)
        P[i].v = (0LL + P[i-1].v + T[i]) % MOD,
        P[i].i = i,
        LP[i].v = (0LL + L[i] + P[i].v) % MOD,
        LP[i].i = i;

    LOG("L :");
    FOR(i, 1, N+1) LOG("%3d", L[i]);
    LOG("\nT: ");
    FOR(i, 1, N+1) LOG("%3d", T[i]);
    LOG("\nP: ");
    FOR(i, 1, N+1) LOG("(%3d, %3d) ", P[i].v, P[i].i);
    LOG("\nLP: ");
    FOR(i, 1, N+1) LOG("(%3d, %3d) ", LP[i].v, LP[i].i);
    LOG("\n\n");

    find_seg(1, N);

    FOR(r, 1, N+1) {
        for (int l : seg[r])
            LOG("%d, %d\n", l, r);
    }

    dp[0] = 1;
    FOR(i, 1, N+1) {
        dp[i] = dp[i-1];
        for (int l : seg[i]) {
            dp[i] = ( 0LL + dp[i] + dp[l-1] ) % MOD;
        }
    }

    FOR(i, 1, N+1) LOG("%3d", dp[i]);
    LOG("\n");

    printf("%d\n", dp[N]);

    return 0;
}
