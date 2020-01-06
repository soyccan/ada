// O(N^2)
// naive DP
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
int L[NN]; // leadership
int T[NN]; // team awareness
int P[NN]; // prefix sum of team awareness
int dp[NN];


int main() {
#ifdef soyccan
    freopen("p1.in", "r", stdin);
#endif
    N = ada::Init();
    FOR(i, 1, N+1) L[i] = ada::GetLeadership();
    FOR(i, 1, N+1) T[i] = ada::GetTeamValue();

    P[0] = 0;
    FOR(i, 1, N+1)
        P[i] = (0LL + P[i-1] + T[i]) % MOD;

    LOG("L :");
    FOR(i, 1, N+1) LOG("%3d", L[i]);
    LOG("\nT: ");
    FOR(i, 1, N+1) LOG("%3d", T[i]);
    LOG("\nP: ");
    FOR(i, 1, N+1) LOG("%3d", P[i]);
    LOG("\n\n");

    dp[0] = 1;
    FOR(i, 1, N+1) {
        dp[i] = dp[i-1];
        FOR(j, 1, i) {
            if (L[j] >= P[i] - P[j])
                dp[i] = ( 0LL + dp[i] + dp[j-1] ) % MOD;
        }
    }

    FOR(i, 1, N+1) LOG("%3d", dp[i]);
    LOG("\n");

    printf("%d\n", dp[N]);

    return 0;
}
