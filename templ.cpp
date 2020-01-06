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
# define INFILE() freopen("in.txt", "r", stdin)
#else
# define LOG(...)
# define INFILE()
#endif

const long long INF = 0x7f7f7f7f7f7f7f7f;

#endif // SOYCCAN_H

using namespace std;

int N;

int main() {
    INFILE();
    while (scanf("%d", &N) != EOF) {
        //
    }
    return 0;
}
