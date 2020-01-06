// TLE version
// also WA on some cases
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#define FOR(i, a, n) for (int i = a; i < n; i++)
#define maximize(var, val) var = max(var, val)
using namespace std;

const int INF = 1000000010;

struct Node {
    // max subarray left/right index
    int msl, msr;
    bool to_update;
    Node *lch, *rch;
    Node(): msl(-1), msr(-1), to_update(true), lch(NULL), rch(NULL) {}
};

int N, Q;
int A[512345];
int bit[512345];

// binary indexed tree
int lowbit(int x) {
    return x & (-x);
}

int presum(int x) {
    int sum = 0;
    for(int i = x; i > 0; i -= lowbit(i))
        sum += bit[i];
    return sum;
}

void add(int x, int val) {
    for (int i = x; i <= N; i += lowbit(i))
        bit[i] += val;
}

int sum(int l, int r) {
    assert(l >= 1 && r >= 1);
    assert(l <= r);
    return presum(r) - presum(l - 1);
}
// end binary indexed tree


int find_max_subarray(Node* node, int l, int r) {
#ifdef soyccan
    fprintf(stderr, "before: l=%d r=%d msl=%d msr=%d, to_update=%d\n", l, r, node->msl, node->msr, (int)node->to_update);
#endif

    if (!node->to_update)
        return sum(node->msl, node->msr);
    node->to_update = false;

    if (l >= r) {
        node->msl = node->msr = l;
        return A[l];
    }

    int m = (l + r) / 2;
    int maxsum = -INF;

    // Case 1: across
    // WARNING: the loop causes TLE, O(N)
    int mxl = -INF, mxr = -INF;
    FOR(i, l, m+1) {
        if (sum(i, m) > mxl) {
            mxl = sum(i, m);
            node->msl = i;
        }
    }
    FOR(i, m+1, r+1) {
        if (sum(m+1, i) > mxr) {
            mxr = sum(m+1, i);
            node->msr = i;
        }
    }
    maxsum = mxl + mxr;

    // Case 2: left
    if (node->lch == NULL) node->lch = new Node();
    int leftmx = find_max_subarray(node->lch, l, m);
    if (leftmx > maxsum) {
        maxsum = leftmx;
        node->msl = node->lch->msl;
        node->msr = node->lch->msr;
    }

    // Case 3: right
    if (node->rch == NULL) node->rch = new Node();
    int rightmx = find_max_subarray(node->rch, m+1, r);
    if (rightmx > maxsum) {
        maxsum = rightmx;
        node->msl = node->rch->msl;
        node->msr = node->rch->msr;
    }

#ifdef soyccan
    fprintf(stderr, "after: l=%d r=%d msl=%d msr=%d, to_update=%d\n", l, r, node->msl, node->msr, (int)node->to_update);
#endif

    return maxsum;
}

void update(int idx, int val, Node* node, int l, int r) {
    if (l <= idx && idx <= r)
        node->to_update = true;

    if (l >= r) {
        add(idx, val - A[idx]);
        A[idx] = val;
        return;
    }

    int m = (l + r) / 2;
    if (idx <= m) update(idx, val, node->lch, l, m);
    else update(idx, val, node->rch, m+1, r);
}

int main() {
#ifdef soyccan
    dup2(open("in.txt", O_RDONLY), 0);
#endif
    while (scanf("%d%d", &N, &Q) == 2) {
        FOR(i, 1, N+1) {
            scanf("%d", &A[i]);
            add(i, A[i]);
        }

        Node* root = new Node();
        printf("%d\n", max(0, find_max_subarray(root, 1, N)));

        while (Q--) {
            int p, v;
            scanf("%d%d", &p, &v);
            update(p, v, root, 1, N);
            printf("%d\n", max(0, find_max_subarray(root, 1, N)));
        }
    }
    return 0;
}
