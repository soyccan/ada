// tree DP
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#ifndef SOYCCAN_H
#define SOYCCAN_H

#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
#define minimize(var, val) var = min(var, val)

#ifdef soyccan
# define LOG(format, ...) fprintf(stderr, format "\n", ##__VA_ARGS__)
# define INFILE() freopen("in.pa", "r", stdin)
#else
# define LOG(...)
# define INFILE()
#endif

const int INF = 0x7f7f7f7f;

#endif // SOYCCAN_H
using namespace std;

const int MAXN = 300000;

struct Vertex {
    int id;
    int dep; // depth
    bool isroot;
    Vertex* pa; // parent
    Vertex* anc;
    // let anc = v,
    // where for all vertex x on the path v->u, including v but not u,
    // out_degree(x) = 1
    // and v has smallest depth

    vector<Vertex*> ed; // edges

    Vertex(): id(-1), dep(-1), isroot(false), pa(NULL), anc(NULL) {}

} G[MAXN]; // reversed graph (a pseudo-forest)
bool vis[MAXN]; // visited
int N;

void dfs(Vertex* u) {
    LOG("u=%d pa=%d dep=%d anc=%d", u->id,u->pa->id,u->dep,u->anc->id);
    for (Vertex* v : u->ed) {
        if (v->dep >= 0) continue;
        v->dep = u->dep + 1;
        v->anc = u->ed.size() == 1 ? u->anc : v;
        dfs(v);
    }
}

Vertex* findroot(const Vertex* u) {
    while (u->pa != 0 && !vis[u->id]) {
        LOG("findroot u=%d",u->id);
        vis[u->id] = true;
        u = u->pa;
    }
    LOG("root=%d", u->id);
    return const_cast<Vertex*>(u);
}

int remove(const Vertex* Set[], int n) {
    int cnt = 0;

    static int od[MAXN]; // out degree

    static Vertex* visV[MAXN];
    int vl = 0;

    static const Vertex* qu[2 * MAXN]; // queue: vertex to remove
    const Vertex** q = qu;
    int l = n;
    FOR(i, 0, n) q[i] = Set[i];

    while (l > 0) {
        LOG("romeve l=%d",l);
        int _l = l;
        FOR(i, 0, l) {
            const Vertex* u = q[i];
            if (u->anc->isroot) continue;

            cnt += u->dep - u->anc->dep + 1;
            if (!vis[u->anc->pa->id]) {
                od[u->anc->pa->id] = u->anc->pa->ed.size();
                vis[u->anc->pa->id] = true;
                visV[vl++] = u->anc->pa;
            }
            LOG("remove u=%d pa=%d anc=%d anc->pa=%d od[anc->pa]=%d",u->id,u->pa->id,u->anc->id,u->anc->pa->id,od[u->anc->pa->id]);
            --od[u->anc->pa->id];

            assert(od[u->anc->pa->id] >= 0);
            if (od[u->anc->pa->id] == 0)
                q[_l++] = u->anc->pa;
        }
        q += l;
        l = _l - l;
    }
    FOR(i, 0, vl) vis[visV[i]->id] = false;
    return cnt;
}

int main() {
//     INFILE();
    setvbuf(stdout, NULL, _IOFBF, 0);
    while (scanf("%d", &N) != EOF) {
        int x, y, q;
        static const Vertex* s[MAXN];

        FOR(i, 1, N+1) {
            scanf("%d", &x);
            G[i].id = i;
            if (x > 0) {
                G[i].pa = &G[x];
                G[x].ed.push_back(&G[i]);
            }
            else {
                G[i].pa = NULL;
            }
        }
        FOR(u, 1, N+1) vis[u] = false;
        FOR(u, 1, N+1) {
            if (G[u].dep >= 0) continue;
            Vertex* r = findroot(&G[u]);
            r->isroot = true;
            r->dep = 0;
            r->anc = r;
            dfs(r);
        }
        scanf("%d", &q);
        FOR(u, 1, N+1) vis[u] = false;
        while (q--) {
            scanf("%d", &x);
            FOR(i, 0, x) {
                scanf("%d", &y);
                s[i] = &G[y];
            }
            printf("%d\n", remove(s, x));
        }
        break;
        FOR(u, 1, N+1) G[u].ed.clear();
        LOG("");
    }
    return 0;
}
