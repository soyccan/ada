// solve Slitherlink by transforming it into CNF-SAT problem
// g++ -Llib -Iinclude -lcryptominisat5 -Wl,-rpath,$PWD/lib -std=c++17 -O2 -march=native ~/ada-19hw4p1.cpp
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <cryptominisat5/cryptominisat.h>

#ifndef SOYCCAN_H
#define SOYCCAN_H

#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
#define minimize(var, val) var = min(var, val)

#ifdef soyccan
# define LOG(format, ...) fprintf(stderr, format "\n", ##__VA_ARGS__)
# define LOGN(format, ...) fprintf(stderr, format, ##__VA_ARGS__)
# define INFILE() freopen("ain", "r", stdin)
#else
# define LOG(...)
# define LOGN(...)
# define INFILE()
# define NDEBUG 1
#endif

const long long INF = 0x7f7f7f7f7f7f7f7f;

#endif // SOYCCAN_H
using namespace std;
using namespace CMSat;

SATSolver solver;
int edge_num; // number of essential varibles = number of edges
int extra_num; // number of extra variables used in conversion
bool sol[35000];
int solN;

int R,C;
char A[130][130];

bool vis[130][130];
int visn = 0;

int loop[35000];
int loopn;

#define addtoclause(cl, var, neg) {\
    LOGN("%s%d ", (neg) ? "-" : "", var+1);\
    (cl).emplace_back(var, neg);}

inline void addComb(int s[], int n, int k, bool allow_empty) {
    /* add all combinations of set s into solver
     * use Tseytin transformation to convert
     * disjuctive normal form (DNF) to conjuctive normal form (CNF)
     * s: set of size n, take k elements
     * n is supposed no larger than 4
     * k is supposed no larger than n
     * total C(n,k) cominations
     * allow_empty: allow that nothing in s is chosen
     *
     * Example:
     * s = {6, 13, 14, 21}
     * n = 4
     * k = 3
     * allow_empty = false
     * => DNF = (6 & 13 & 14) | (6 & 13 & 21) | (6 & 14 & 21) | (13 & 14 & 21)
     */

    assert(n <= 4);
    assert(k <= n);

    vector<Lit> cl;
    int xn = edge_num + extra_num;

    static bool fil[4];
    FOR(i, 0, n-k) fil[i] = false;
    FOR(i, n-k, n) fil[i] = true;

    do {
        solver.new_var(); // Note the efficiency cost

        FOR(i, 0, n) {
            if (fil[i]) {
                cl.clear();
                addtoclause(cl, xn, true);
                addtoclause(cl, s[i]-1, false);
                solver.add_clause(cl);
                LOG("");
            }
            else {
                cl.clear();
                addtoclause(cl, xn, true);
                addtoclause(cl, s[i]-1, true);
                solver.add_clause(cl);
                LOG("");
            }
        }

        cl.clear();
        addtoclause(cl, xn, false);
        FOR(i, 0, n) {
            if (fil[i]) {
                addtoclause(cl, s[i]-1, true);
            }
            else {
                addtoclause(cl, s[i]-1, false);
            }
        }
        LOG("");
        solver.add_clause(cl);

        ++xn;
    } while (next_permutation(fil, fil+n));

    if (allow_empty) {
        solver.new_var(); // Note the efficiency cost

        FOR(i, 0, n) {
            cl.clear();
            addtoclause(cl, xn, true);
            addtoclause(cl, s[i]-1, true);
            LOG("");
            solver.add_clause(cl);
        }

        cl.clear();
        addtoclause(cl, xn, false);
        FOR(i, 0, n)
            addtoclause(cl, s[i]-1, false);
        LOG("");
        solver.add_clause(cl);

        ++xn;
    }

    cl.clear();
    FOR(i, edge_num + extra_num, xn) {
        addtoclause(cl, i, false);
    }
    LOG("");
    solver.add_clause(cl);

    extra_num = xn - edge_num;
}

inline void getedgeidx(
        int i, int j, int& up, int& down,
        int& left, int& right, int& leftup, int& upleft) {
    up = 1 + j + i*(2*C + 1);
    down = up + 2*C + 1;
    left = up + C;
    right = left + 1;
    leftup = left - 2*C - 1;
    upleft = up - 1;

    if (i == R) {
        left = -1;
        up = i * (2*C + 1) + j + 1;
        upleft = up - 1;
        leftup = up - (C+1);
    }

    if (j == C) {
        left = (i+1) * (2*C + 1);
        leftup = i * (2*C + 1);
        upleft = left - (C+1);
        up = -1;
    }

    if (i == R && j == C) {
        up = left = -1;
        upleft = 2*R*C + R + C;
        leftup = upleft - C;
    }

    if (i == 0)
        leftup = -1;

    if (j == 0)
        upleft = -1;
}

inline void addcellvertex(int i, int j) {
    /* the upper-left vertex of this cell
     * should have exactly 0 or 2 edges
     */
    int up, down, left, right, leftup, upleft;
    getedgeidx(i, j, up, down, left, right, leftup, upleft);

    static int edge[4];
    static int tmpedge[4];

    edge[0] = leftup;
    edge[1] = upleft;
    edge[2] = left;
    edge[3] = up;
    LOG("vertex (%d,%d) %d %d %d %d",i,j,leftup,upleft,left,up);

    int tn = 0;
    FOR(i, 0, 4)
        if (edge[i] > 0)
            tmpedge[tn++] = edge[i];

    addComb(tmpedge, tn, 2, true);
}

inline void addcelledge(int i, int j, int n) {
    /* the A[i][j] cell has exacly n coloured edge */

    if (i >= R || j >= C) return;

    int up, down, left, right, leftup, upleft;
    getedgeidx(i, j, up, down, left, right, leftup, upleft);

    // 4 edges of this cell
    static int edge[4];
    edge[0] = up;
    edge[1] = down;
    edge[2] = left;
    edge[3] = right;
    LOG("cell   (%d,%d) %d %d %d %d",i,j,up,down,left,right);

    addComb(edge, 4, n, false);
}

inline void init() {
//     solver.set_num_threads(4);

    edge_num = 2*R*C + R + C;
    solver.new_vars(edge_num);
}

void dfs(int i, int j) {
    if (i < 0 || i > R || j < 0 || j > C || vis[i][j]) return;

    LOG("dfs %d %d visn=%d solN=%d loopn=%d", i,j,visn,solN,loopn);
    vis[i][j] = true;
    ++visn;

    int up, down, left, right, leftup, upleft;
    getedgeidx(i, j, up, down, left, right, leftup, upleft);

    if (up     > 0 && sol[up])     { loop[loopn++] =     up; dfs(i, j+1); }
    if (left   > 0 && sol[left])   { loop[loopn++] =   left; dfs(i+1, j); }
    if (upleft > 0 && sol[upleft]) { loop[loopn++] = upleft; dfs(i, j-1); }
    if (leftup > 0 && sol[leftup]) { loop[loopn++] = leftup; dfs(i-1, j); }
}

inline bool one_loop() {
    visn = loopn = 0;
    memset(vis, false, sizeof vis);
    FOR(i, 0, R) {
        FOR(j, 0, C) {
            int up, down, left, right, leftup, upleft;
            getedgeidx(i, j, up, down, left, right, leftup, upleft);

            if ( !vis[i][j] && (
                    (up > 0 && sol[up])
                    || (left > 0 && sol[left])
                    || (leftup > 0 && sol[leftup])
                    || (upleft > 0 && sol[upleft]))) {
                dfs(i, j);
                if (visn != solN) {
                    LOG("not one loop, loopn=%d", loopn);

                    // Banning one of the loops found
                    vector<Lit> ban_loop;
                    FOR(i, 0, loopn) {
                        addtoclause(ban_loop, loop[i]-1, true);
                    }
                    solver.add_clause(ban_loop);

                    visn = loopn = 0;
                }
                else {
                    LOG("one loop");
                    return true;
                }
            }
        }
    }
    return false;

}

inline void visualize() {
    const vector<lbool>& ans = solver.get_model();

    int idx = 1;
    FOR(i, 0, R+1) {
        FOR(j, 0, C) {
            LOGN("┼");
            if (ans[idx - 1] == l_True)
                LOGN("─");
            else
                LOGN(" ");
            ++idx;
        }
        LOG("┼");

        if (i == R) break;

        FOR(j, 0, C+1) {
            if (ans[idx - 1] == l_True)
                LOGN("│");
            else
                LOGN(" ");

            if (i < R && j < C)
                LOGN("%c", A[i][j]);

            ++idx;
        }
        LOG("");
    }
}

inline void findsol() {
    // Note: unique solution is guaranteed
    //
    while (true) {
        lbool ret = solver.solve();
        if (ret != l_True) {
            // All solutions found.
            assert(ret == l_False);
            return;
        }

        const vector<lbool>& model = solver.get_model();

        solN = 0;
        memset(sol, 0, sizeof sol);
        LOG("edge: ");
        FOR(i, 0, edge_num) {
            if (model[i] == l_True) {
                sol[i+1] = true;
                LOGN("%d ", i+1);
            }
        }
        LOG("");

        visualize();

        if (one_loop()) {
            LOG("one loop");

            FOR(i, 1, edge_num+1)
                printf("%d", sol[i]);
            puts("");

            break;
        }
        LOG("");
    }
}

int main() {
//     INFILE();
    scanf("%d%d", &R, &C);
    init();
    FOR(i, 0, R) {
        FOR(j, 0, C) {
            char c;
            while ( (c = getchar()) == '\n' );
            if ('0' <= c && c <= '4') {
                addcelledge(i, j, c - '0');
            }
            A[i][j] = c;
        }
    }
    FOR(i, 0, R+1)
        FOR(j, 0, C+1)
            addcellvertex(i, j);
    findsol();
    return 0;
}

