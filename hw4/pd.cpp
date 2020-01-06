// Monte Carlo Method
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <utility>
#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "hex.h"

#ifndef SOYCCAN_H
#define SOYCCAN_H

#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
#define minimize(var, val) var = min(var, val)

#ifdef soyccan
# define LOG(format, ...) fprintf(stderr, format "\n", ##__VA_ARGS__)
# define INFILE() freopen("in.txt", "r", stdin)
#else
# define LOG(...)
# define INFILE()
#endif

const long long INF = 0x7f7f7f7f7f7f7f7f;

#endif // SOYCCAN_H

using namespace std;

typedef pair<int, int> pii;

const int MAXN = 13;

int N;
int B[MAXN][MAXN]; // board
int C[MAXN][MAXN]; // copy of board
int num_left;

int border;
bool vis[MAXN][MAXN];

int rate[MAXN][MAXN];

void _print() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < i; ++j)
                std::cerr << " ";
            for (int j = 0; j < N; ++j) {
                int st = C[i][j];
                if (st == 1) std::cerr << "O ";
                else if (st == 2) std::cerr << "X ";
                else std::cerr << "_ ";
            }
            std::cerr << std::endl;
        }
}

void dfs(int x, int y) {
//     LOG("dfs %d %d",x,y);
    if (vis[x][y]) return;
    if (x == 0) border |= 1;
    if (x == N-1) border |= 2;
    if (y == 0) border |= 4;
    if (y == N-1) border |= 8;

    vis[x][y] = true;

    int adj[6][2] = {
        { x+1, y },
        { x-1, y },
        { x, y+1 },
        { x, y-1 },
        { x-1, y+1 },
        { x+1, y-1 }
    };
    FOR(i, 0, 6) {
        int nx = adj[i][0];
        int ny = adj[i][1];
        if (0 <= nx && nx < N
                && 0 <= ny && ny < N
                && C[nx][ny] == C[x][y])
            dfs(nx, ny);
    }
}

int checkpath(int x, int y) {
    border = 0;
    memset(vis, false, sizeof vis);
    dfs(x, y);
//     LOG("border %d win? %d %d",border,(border & 3) == 3,(border & 12) == 12);
//     LOG("");
    if ( (border & 3) == 3 && C[x][y] == 1)
        return 1; // O wins
    else if ( (border & 12) == 12 && C[x][y] == 2)
        return 2; // X wins
    else
        return 0; // nothing
}

void init(int n) {
    N = n;
    num_left = n * n;
    memset(B, 0, sizeof B);
    srand(12345);
}
pii decide(pii p) {
//     if (p != make_pair(-1, -1))
//         B[p.first][p.second] = true;
//     vector<pii> unused;
//     for (int i = 0; i < N; ++i)
//         for (int j = 0; j < N; ++j)
//             if (!B[i][j])
//                 unused.push_back({i, j});
//     pii res = unused[rand() % unused.size()];
//     B[res.first][res.second] = true;
//     return res;

    B[ p.first ][ p.second ] = 2;

    int num_round = 100;
    int num_step = num_left;
    memset(rate, 0, sizeof rate);

    FOR(r, 0, num_round) {
//         LOG("r=%d",r);
        FOR(i, 0, N) {
            FOR(j, 0, N) {
                if (B[i][j] > 0) continue;

                // start at (i,j)
                memcpy(C, B, sizeof C);
//                 LOG("start at (%d,%d)",i,j);

                int ci = i;
                int cj = j;
                bool color = 0; // O=1; X=2; O first

                FOR(s, 0, num_step) {
                    C[ci][cj] = color + 1;
//                     LOG("(%d,%d) = %d", ci, cj, color+1);
                    color = !color;

//                     _print();

                    int w;
                    if ( (w = checkpath(ci, cj)) > 0) {
//                         LOG("path w=%d %d %d %d %d",w,ci,cj,i,j);
                        if (w == 1)
                            ++rate[i][j];
                        break;
                    }

                    // choose next step
                    // TODO
                    ci = rand() % N;
                    cj = rand() % N;
                    if (C[ci][cj] > 0) {
                        // Note effieciency
                        FOR(_i, ci, N)
                            FOR(_j, 0, N)
                                if (C[_i][_j] == 0)
                                    ci = _i, cj = _j;
                        if (C[ci][cj] > 0) {
                            for (int _i = ci; _i >= 0; --_i)
                                for (int _j = N-1; _j >= 0; --_j)
                                    if (C[_i][_j] == 0)
                                        ci = _i, cj = _j;
                        }
                    }
                    assert(C[ci][cj] == 0);
                }
            }
        }
    }
    pii ret;
    int mx = 0;
    FOR(i, 0, N)
        FOR(j, 0, N)
            if (rate[i][j] > mx) {
                mx = rate[i][j];
                ret = make_pair(i, j);
            }
    B[ ret.first ][ ret.second ] = 1;
    --num_left;
    return ret;
}
