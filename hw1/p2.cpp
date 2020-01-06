// segment-tree
// https://codeforces.com/blog/entry/56416
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
    // max prefix, max suffix, max subarray sum, total sum
    long long mxpre, mxsuf, mxsub, sum;

    // to update flag
    bool up;

    Node *lch, *rch;

    Node() {
        lch = rch = NULL;
        mxpre = mxsuf = mxsub = -INF;
        sum = 0;
        up = true;
    }
};

int N, Q;
int A[512345];

long long find_max_subarray(Node* node, int l, int r) {
#ifdef soyccan
    fprintf(stderr, "before: l=%d r=%d mxpre=%d mxsuf=%d mxsub=%d sum=%d up=%d\n",
            l, r, node->mxpre, node->mxsuf, node->mxsub, node->sum, (int)node->up);
    assert(l <= r);
#endif

    if (!node->up)
        return node->mxsub;
    node->up = false;

    if (l == r) {
        node->mxpre = node->mxsuf = node->mxsub = node->sum = A[l];
        return A[l];
    }

    int m = (l + r) / 2;
    if (node->lch == NULL) node->lch = new Node();
    if (node->rch == NULL) node->rch = new Node();
    find_max_subarray(node->lch, l, m);
    find_max_subarray(node->rch, m+1, r);

    node->sum = node->lch->sum + node->rch->sum;

    node->mxpre = max(
            node->lch->mxpre,
            node->lch->sum + node->rch->mxpre);

    node->mxsuf = max(
            node->lch->mxsuf + node->rch->sum,
            node->rch->mxsuf);

    node->mxsub = max({
            node->lch->mxsub,
            node->rch->mxsub,
            node->lch->mxsuf + node->rch->mxpre});

#ifdef soyccan
    fprintf(stderr, "after: l=%d r=%d mxpre=%d mxsuf=%d mxsub=%d sum=%d up=%d\n",
            l, r, node->mxpre, node->mxsuf, node->mxsub, node->sum, (int)node->up);
#endif

    return node->mxsub;
}

void update(int idx, int val, Node* node, int l, int r) {
    assert(l <= r);

    if (l <= idx && idx <= r)
        node->up = true;

    if (l == r) {
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
        }

        Node* root = new Node();
        printf("%lld\n", max(0LL, find_max_subarray(root, 1, N)));

        while (Q--) {
            int p, v;
            scanf("%d%d", &p, &v);
            update(p, v, root, 1, N);
            printf("%lld\n", max(0LL, find_max_subarray(root, 1, N)));
        }
    }
    return 0;
}
