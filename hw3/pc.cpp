// cycle detection
// DAG shortest path
#include <cassert>
#include <cstdio>
#include <algorithm>
#include <utility>
#include <vector>
#include <tuple>

#ifndef SOYCCAN_H
#define SOYCCAN_H

#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
#define minimize(var, val) var = min(var, val)

#ifdef soyccan
# define LOG(format, ...) fprintf(stderr, format "\n", ##__VA_ARGS__)
# define INFILE() freopen("cin", "r", stdin)
#else
# define LOG(...)
# define INFILE()
# define NDEBUG
#endif

const long long INF = 0x7f7f7f7f7f7f7f7f;

#endif // SOYCCAN_H

using namespace std;

struct Song {
    int h; // head vertex id
    int t; // tail vertex id
    int l; // length
};

struct Vertex {
    int i;  // id
    int s; // song id
    int n; // note index
    int dt; // discover time
    int ft; // finish time
    long long d; // distance
};

struct Edge {
    int v; // vertex id
    int w; // edge weight
    Edge(int v, int w): v(v), w(w) {}
};

const int MAXV = 1312345;
const int MAXN = 112345;
const int MAXM = 512345;

Song A[MAXN];
vector<pair<int,int>> jmp[MAXN];
// jmp[ song_id ] = [ (note_idx_1, vertex_id_1), ... ]
int N;

vector<Edge> G[MAXV];
Vertex vtx[MAXV];
int V; // number of vertices

void init() {
    FOR(i, 0, V) G[i].clear();
    FOR(i, 0, N+1) jmp[i].clear();
    V = 0;
}

int addvertex(int song, int note) {
    vtx[V].i = V;
    vtx[V].s = song;
    vtx[V].n = note;
    return V++;
}

void addedge(int u, int v, int w) {
    LOG("addedge u=(%d,%d,%d) v=(%d,%d,%d) w=%d",u,vtx[u].s,vtx[u].n,v,vtx[v].s,vtx[v].n,w);
    // Note: store additive inverse of weight
    G[u].emplace_back(v, -w);
}

bool toposort(int u, int& tmstp, int& tpidx, int svtx[], bool vis[]) {
    vtx[u].dt = tmstp++;
    LOG("dfs d=%d dt=%d ft=%d u=%d s=%d n=%d",vtx[u].d,vtx[u].dt,vtx[u].ft,u,vtx[u].s,vtx[u].n);

    for (Edge& e : G[u]) {
        int v = e.v;
        LOG("  dfs d=%d dt=%d ft=%d v=%d s=%d n=%d",vtx[v].d,vtx[v].dt,vtx[v].ft,v,vtx[v].s,vtx[v].n);

        if (vtx[v].dt > 0 && vtx[v].ft == -1)
            // back edge
            return false;

        if (vis[v]) continue;
        vis[v] = true;

        if (!toposort(v, tmstp, tpidx, svtx, vis))
            return false;
    }

    assert(tpidx >= 0);
    svtx[tpidx--] = u;
    vtx[u].ft = tmstp++;
    return true;
}

bool dag_shortest_path(int s) {
    int tmstp = 0; // time stamp
    int tpidx = V-1;
    static int svtx[MAXV]; // sorted vertices
    static bool vis[MAXV];
    FOR(u, 0, V) {
        vis[u] = false;
        vtx[u].d = INF;
        vtx[u].dt = vtx[u].ft = -1;
    }
    if (!toposort(s, tmstp, tpidx, svtx, vis))
        return false;

    vtx[s].d = 0;
    FOR(i, 0, V) {
        int u = svtx[i];
        LOG("dag d=%d dt=%d ft=%d u=%d s=%d n=%d",vtx[u].d,vtx[u].dt,vtx[u].ft,u,vtx[u].s,vtx[u].n);
        for (Edge& e : G[u]) {
            int v = e.v;
            LOG("   d=%d dt=%d ft=%d v=%d s=%d n=%d",vtx[v].d,vtx[v].dt,vtx[v].ft,v,vtx[v].s,vtx[v].n);
            if (vtx[v].d > vtx[u].d + e.w) {
                vtx[v].d = vtx[u].d + e.w;
            }
        }
    }
    return true;
}

int main() {
	INFILE();
    int M,T;
    int s1, t1, s2, t2;
    static int tmp[MAXM][4];
    // [ (s1, t1, s2, t2) ... ]
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &N, &M);
        init();
        int root = addvertex(-1, -1);
        FOR(i, 1, N+1) {
            scanf("%d", &A[i].l);
            A[i].h = addvertex(i, 1);
            A[i].t = addvertex(i, A[i].l);
            addedge(root, A[i].h, 1);

            jmp[i].push_back(make_pair(1, A[i].h));
            jmp[i].push_back(make_pair(A[i].l, A[i].t));
        }
        FOR(i, 0, M) {
            scanf("%d%d%d%d", &s1, &t1, &s2, &t2);
            tie(tmp[i][0], tmp[i][1], tmp[i][2], tmp[i][3]) = tie(s1, t1, s2, t2);
            jmp[s1].push_back(make_pair(t1, -1));
            jmp[s2].push_back(make_pair(t2, -1));
        }
        FOR(i, 1, N+1) {
            sort(jmp[i].begin(), jmp[i].end());
            jmp[i].erase(unique(jmp[i].begin(), jmp[i].end()), jmp[i].end());

            FOR(j, 0, jmp[i].size()-1) {
                if (jmp[i][j].second == -1)
                    jmp[i][j].second = addvertex(i, jmp[i][j].first);

                if (jmp[i][j+1].second == -1)
                    jmp[i][j+1].second = addvertex(i, jmp[i][j+1].first);

                int u = jmp[i][j].second;
                int v = jmp[i][j+1].second;
                addedge(u, v, vtx[v].n - vtx[u].n);
            }
        }
        FOR(i, 0, M) {
            tie(s1, t1, s2, t2) = tie(tmp[i][0], tmp[i][1], tmp[i][2], tmp[i][3]);

            auto j1 = lower_bound(jmp[s1].begin(), jmp[s1].end(), make_pair(t1, -1));
            assert(j1 != jmp[s1].end());
            int u1 = j1->second;

            auto j2 = lower_bound(jmp[s2].begin(), jmp[s2].end(), make_pair(t2, -1));
            assert(j2 != jmp[s2].end());
            int u2 = j2->second;

            addedge(u1, u2, 1);
        }
        if (dag_shortest_path(0)) {
            long long ans = INF;
            FOR(i, 0, V) minimize(ans, vtx[i].d);
            printf("%lld\n", -ans);
        }
        else {
            printf("LoveLive!\n");
        }
    }
    return 0;
}
