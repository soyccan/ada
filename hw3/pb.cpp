// dijkstra
// widest path + shortest path
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <utility>
#include <queue>
#include <vector>

#ifndef SOYCCAN_H
#define SOYCCAN_H

#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
#define minimize(var, val) var = min(var, val)

#ifdef soyccan
# define LOG(format, ...) fprintf(stderr, format "\n", ##__VA_ARGS__)
# define INFILE() freopen("bin", "r", stdin)
# define FIN() fin()
#else
# define LOG(...)
# define INFILE()
# define FIN()
#endif

const long long INF = 0x7f7f7f7f7f7f7f7f;

#endif // SOYCCAN_H

using namespace std;

struct Edge {
    int v; // to vertex
    int d; // distance
    int l; // level of danger
    Edge() {}
    Edge(int v, int d, int l): v(v), d(d), l(l) {}
};

const int MAXN = 200010;
const int MAXM = 500010;
int N,M;

vector<Edge> G[MAXN];

long long dis[MAXN];
int bot[MAXN]; // maximum bottle neck of level of danger
bool done[MAXN];

void dijkstra(int s, int gbot) {
    // s: source
    // gbot: global bottleneck from s to t
    //       -1: to minimize bottleneck
    //       >0: to minimize distance without making bottleneck worse
    FOR(i, 0, N) {
        done[i] = false;
        dis[i] = INF;
        bot[i] = INF;
    }

    priority_queue<pair<long long,int>> q; // key: -bot[v] or -dis[v]
    done[s] = true;
    dis[s] = 0;
    bot[s] = 0;
    if (gbot < 0)
        q.push(make_pair(-bot[s], s));
    else
        q.push(make_pair(-dis[s], s));

    while (!q.empty()) {
        int u = q.top().second; q.pop();
        done[u] = true;

        LOG("u=%d dis=%d bot=%d",u,dis[u],bot[u]);

        for (Edge e : G[u]) {
            int v = e.v;
            if (done[v]) continue;

            LOG("  before: v=%d dis=%d bot=%d",v,dis[v],bot[v]);
            int b = max(bot[u], e.l); // new bottleneck
            if (gbot < 0) {
                if (bot[v] > b) {
                    bot[v] = b;
                    dis[v] = dis[u] + e.d;
                    q.push(make_pair(-bot[v], v));
                }
                else if (bot[v] == b && dis[v] > dis[u] + e.d) {
                    dis[v] = dis[u] + e.d;
                    q.push(make_pair(-bot[v], v));
                }
            }
            else {
                if (b <= gbot && dis[v] > dis[u] + e.d) {
                    bot[v] = b;
                    dis[v] = dis[u] + e.d;
                    q.push(make_pair(-dis[v], v));
                }
            }
            LOG("  after: v=%d dis=%d bot=%d",v,dis[v],bot[v]);
        }
    }
}

void fin() {
    FOR(u, 0, N) {
        G[u].clear();
    }
}

int main() {
	INFILE();
    int s,t,x,y,d,l;
    while (scanf("%d%d%d%d", &N, &M, &s, &t) != EOF) {
        FOR(i, 0, M) {
            scanf("%d%d%d%d", &x, &y, &d, &l);
            G[x].emplace_back(y, d, l);
            G[y].emplace_back(x, d, l);
        }
        dijkstra(s, -1);
        dijkstra(s, bot[t]);
        printf("%lld %d\n", dis[t], bot[t]);
        FIN();
    }
    return 0;
}

