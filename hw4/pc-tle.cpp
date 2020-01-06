// Bron-Kerbosch algorithm
// maximum clique
// TLE
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <utility>

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
# define NDEBUG 1
#endif

const long long INF = 0x7f7f7f7f7f7f7f7f;

#endif // SOYCCAN_H

using namespace std;

const int MAXV = 122;
bool G[MAXV][MAXV]; // adjacency matrix
bool self_cycle[MAXV];
int sorted[MAXV]; // sorted vertices by degeneracy order
int V;

struct Set {
    unsigned long long s[2]; // bitset
    size_t sz; // size
};

inline bool setContain(const Set* s, int u) {
    assert(u < MAXV);
    if (u < 64)
        return (s->s[0] >> u) & 1;
    else
        return (s->s[1] >> (u - 64)) & 1;
}

inline void setEmpty(Set* s) {
    s->s[0] = s->s[1] = 0;
    s->sz = 0;
}

inline void setFull(Set* s) {
    s->s[0] = s->s[1] = 0xffffffffffffffffULL;
    s->sz = V;
}

inline void setAdd(Set* s, int u) {
    assert(u < MAXV);
    if (setContain(s, u)) return;

    ++s->sz;
    if (u < 64)
        s->s[0] |= 1ULL << u;
    else
        s->s[1] |= 1ULL << (u - 64);
}

inline void setRemove(Set* s, int u) {
    assert(u < MAXV);
    if (!setContain(s, u)) return;

    --s->sz;
    if (u < 64)
        s->s[0] &= ~(1ULL << u);
    else
        s->s[1] &= ~(1ULL << (u - 64));
}

inline void setCopy(Set* dest, const Set* src) {
    dest->s[0] = src->s[0];
    dest->s[1] = src->s[1];
    dest->sz   = src->sz;
}

inline void setIntersectNeighbor(Set* s, int u) {
    FOR(i, 0, V) {
        if (setContain(s, i) && !G[u][i]) {
            setRemove(s, i);
        }
    }
}

void backtrack(Set* R, Set* P, Set* X, Set* res) {
//     printf("\n\nself=%d\nR: ", self_cycle_num);
//     FOR(u, 0, V)
//         if (setContain(R, u))
//             printf("%d ", u);
//     printf("\nP: ");
//     FOR(u, 0, V)
//         if (setContain(P, u))
//             printf("%d ", u);
//     printf("\nX: ");
//     FOR(u, 0, V)
//         if (setContain(X, u))
//             printf("%d ", u);

    if (P->sz == 0) {
        if (X->sz == 0 && R->sz > 1) {
//             LOG("found clique");
//             FOR(u, 0, V) {
//                 if (setContain(R, u)) {
//                     LOG("in clique: %d", u);
//                 }
//             }

            if (res->sz < R->sz) {
                res->sz = R->sz;
                setCopy(res, R);
            }
        }
        return;
    }

    int pivot = 0;
    while (pivot < V
            && !setContain(P, pivot)
            && !setContain(X, pivot))
        ++pivot;

    FOR(u, 0, V) {
        if (!G[pivot][u] && setContain(P, u)) {
            Set _R, _P, _X;
            setCopy(&_R, R);
            setCopy(&_P, P);
            setCopy(&_X, X);

            setAdd(&_R, u);
            setIntersectNeighbor(&_P, u);
            setIntersectNeighbor(&_X, u);

            backtrack(&_R, &_P, &_X, res);

            setRemove(P, u);
            setAdd(X, u);
        }
    }
}

void degen_sort() {
    /* sort vetices by degeneracy order */
    static bool _G[MAXV][MAXV];
    static int deg[MAXV]; // degree
    static bool used[MAXV];
    FOR(i, 0, V) deg[i] = 0, used[i] = false;
    FOR(i, 0, V) {
        FOR(j, 0, V) {
            _G[i][j] = G[i][j];
            if (G[i][j]) {
                ++deg[i];
                ++deg[j];
            }
        }
    }
    int n = 0;
    while (n < V) {
        int u = -1;
        int d = INF;
        FOR(i, 0, V) {
            if (d > deg[i] && !used[i]) {
                d = deg[i];
                u = i;
            }
        }
        sorted[n++] = u;
        used[u] = true;
        FOR(i, 0, V) {
            if (_G[u][i]) {
                _G[u][i] = _G[i][u] = false;
                deg[i] -= 2;
                deg[u] -= 2;
            }
        }
    }
}

void BronKerbosch() {
    // remove self cycle
    FOR(i, 0, V) self_cycle[i] = false;
    FOR(i, 0, V) {
        if (G[i][i]) {
            self_cycle[i] = true;
            G[i][i] = false;
        }
    }

    // make undirected
    FOR(i, 0, V)
        FOR(j, i+1, V)
            if (!G[i][j] || !G[j][i])
                G[i][j] = G[j][i] = false;

    degen_sort();
//     FOR(i, 0, V) printf("%d ", sorted[i]);
//     puts("");

    Set R, P, X, max_clique;
    setEmpty(&R);
    setFull(&P);
    setEmpty(&X);
    setEmpty(&max_clique);
    FOR(u, 0, V) if (!self_cycle[u]) setRemove(&P, u);

    FOR(i, 0, V) {
        int u = sorted[i];

        Set _P, _X;
        setCopy(&_P, &P);
        setCopy(&_X, &X);

        setEmpty(&R);
        setAdd(&R, u);
        setIntersectNeighbor(&_P, u);
        setIntersectNeighbor(&_X, u);

        backtrack(&R, &_P, &_X, &max_clique);

        setRemove(&P, u);
        setAdd(&X, u);
    }

    static bool used[MAXV];
    static int perm[MAXV];
    int n = 0;
    FOR(i, 0, V) used[i] = false;
    FOR(i, 0, V) {
        if (setContain(&max_clique, i)) {
            perm[n++] = i;
            used[i] = true;
        }
    }
    FOR(i, 0, V) {
        if (!used[i]) {
            perm[n++] = i;
            used[i] = true;
        }
    }

    FOR(i, 0, n)
        printf("%d%s", perm[i]+1, i == n-1 ? "\n" : " ");

    // debug
//     FOR(i, 0, V) if (self_cycle[i]) G[i][i] = true;
//     LOG("before permute:");
//     FOR(i, 0, V) {
//         FOR(j, 0, V)
//             fprintf(stderr, "%d ", G[i][j]);
//         LOG("");
//     }
//     LOG("after permute:");
//     FOR(i, 0, V) {
//         FOR(j, 0, V)
//             fprintf(stderr, "%d ", G[perm[i]][perm[j]]);
//         LOG("");
//     }
}

int main() {
//     INFILE();
    while (scanf("%d", &V) != EOF) {
        FOR(i, 0, V) {
            FOR(j, 0, V) {
                int c = -1;
                while ( (c = getchar()) == '\n' );
                if (c == '1')
                    G[i][j] = true;
                else if (c == '0')
                    G[i][j] = false;
                else
                    assert(false);
            }
        }
        BronKerbosch();
    }
    return 0;
}
