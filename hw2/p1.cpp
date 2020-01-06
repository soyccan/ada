// O(N lg N)
// DP + binary search
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <utility>

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


const int MOD = 1000000007;
const size_t NN = 2123456;
int N;
int L[NN];       // leadership
                 // 1 <= i <= N
int T[NN];       // team awareness
                 // 1 <= i <= N
long long P[NN]; // prefix sum of team awareness
                 // 1 <= i <= N
int dp[NN];      // dp[i] : ways to form a combination of fleets, for ship i to ship N
                 // 1 <= i <= N+1
int dps[NN];     // suffix sum of dp
                 // 1 <= i <= N+1


int find_index(int st) {
    int l = st; // start
    int r = N;
    while (l < r) {
        int m = (l + r + 1) / 2;
        if (L[st] >= P[m] - P[st])
        // NOTE: if P store value after modulo, here comes error
            l = m;
        else
            r = m-1;
    }
    assert(l == r);
    return l;
}

int main() {
#ifdef soyccan
    freopen("p1.in", "r", stdin);
#endif
    N = ada::Init();
    FOR(i, 1, N+1) L[i] = ada::GetLeadership();
    FOR(i, 1, N+1) T[i] = ada::GetTeamValue();

    P[0] = 0;
    FOR(i, 1, N+1)
        P[i] = P[i-1] + T[i];

    LOG("L :");
    FOR(i, 1, N+1) LOG("%3d", L[i]);
    LOG("\nT: ");
    FOR(i, 1, N+1) LOG("%3d", T[i]);
    LOG("\nP: ");
    FOR(i, 1, N+1) LOG("%3d", P[i]);
    LOG("\n\n");

    dps[N+2] = dp[N+2] = 0;
    dps[N+1] = dp[N+1] = 1;
    for (int i = N; i >= 1; i--) {
        int idx = find_index(i);
        dp[i] = (0LL + dps[i+1] - dps[idx+2] + MOD) % MOD;
        dps[i] = (0LL + dps[i+1] + dp[i]) % MOD;
    }

    FOR(i, 1, N+1) LOG("%3d", dp[i]);
    LOG("\n");

    printf("%d\n", dp[1]);

    return 0;
}
