// MinHash & Jaccard similarity
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <cstdint>
#include <random>

#ifndef SOYCCAN_H
#define SOYCCAN_H

#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
#define minimize(var, val) var = min(var, val)

#ifdef soyccan
# define LOG(format, ...) fprintf(stderr, format "\n", ##__VA_ARGS__)
# define INFILE() freopen("bin", "r", stdin)
#else
# define LOG(...)
# define INFILE()
#endif

const long long INF = 0x7f7f7f7f7f7f7f7f;

#endif // SOYCCAN_H

using namespace std;

const int HASH_ROUND = 600;
const int MAXN = 200005;

uint32_t mH[MAXN][HASH_ROUND]; // MinHash

vector<int> G[MAXN];
bool vis[MAXN];

int ts = 0; // timestamp
int tf[MAXN]; // time finished in DFS

bool inV[MAXN];
int V[MAXN];
int Vn;

int N,M,Q;

mt19937 gen;
uniform_int_distribution<uint32_t> dis;


void dfs(int u) {

    FOR(i, 0, HASH_ROUND) {
        mH[u][i] = dis(gen);
        LOG("mH[%d][%d] = %u", u,i,mH[u][i]);
    }
    vis[u] = true;

    for (int v : G[u]) {
        if (!vis[v])
            dfs(v);
        FOR(i, 0, HASH_ROUND)
            minimize(mH[u][i], mH[v][i]);
    }
    tf[u] = ++ts;
}

void genhash() {
    ts = 0;
    FOR(j, 0, Vn)
        if (!vis[ V[j] ])
            dfs(V[j]);
}

int main() {
    INFILE();
    scanf("%d%d%d", &N, &M, &Q);
    int u, v;
    while (M--) {
        scanf("%d%d", &u, &v);
        G[u].push_back(v);
        inV[u] = inV[v] = true;
    }
    FOR(i, 1, N+1) if (inV[i]) V[Vn++] = i;
    genhash();
    while (Q--) {
        scanf("%d%d", &u, &v);
        int cnt = 0;
        if ( !inV[u] || !inV[v]
                || (tf[u] > tf[v] && G[u].empty())
                || (tf[u] < tf[v] && G[v].empty()) ) {
            printf("0.000000\n");
        }
        else if (u == v) {
            printf("1.000000\n");
        }
        else {
            FOR(i, 0, HASH_ROUND) {
                LOG("i=%d h[%d]=%u h[%d]=%u",i,u,mH[i][u],v,mH[i][v]);
                if (mH[u][i] == mH[v][i]) {
                    ++cnt;
                }
            }
            printf("%f\n", (float)cnt / HASH_ROUND);
        }
    }
    return 0;
}
