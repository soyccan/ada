#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>
#include <cstring>
#include "hex.h"

using namespace std;
typedef pair<int, int> pii;

const int maxn = 11;

bool board[maxn][maxn];

int n;

void init(int _n) {
    n = _n;
    memset(board, 0, sizeof board);
    srand(12345);
}
pii decide(pii p) {
    if (p != make_pair(-1, -1))
        board[p.first][p.second] = true;
    vector<pii> unused;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (!board[i][j])
                unused.push_back({i, j});
    pii res = unused[rand() % unused.size()];
    board[res.first][res.second] = true;
    return res;
}
