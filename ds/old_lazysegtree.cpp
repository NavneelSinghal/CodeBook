// see problem - https://codeforces.com/contest/1477/problem/B
// ^ for another solution

const int N = 100100;

struct node {
    // heads = number of heads, lazy = whether we need to flip or not
    int heads, lazy;
    node() { heads = 0, lazy = 0; }
};

// how to combine two nodes
node merge(node &a, node &b) {
    node x;
    x.heads = a.heads + b.heads;
    return x;
}

// how to update a given node
void flip(node &a, int l, int r) {
    a.heads = r - l + 1 - a.heads;
    a.lazy ^= 1;
}

node t[4 * N];

// edit this properly
void push(int v, int tl, int tm, int tr) {
    if (t[v].lazy) {
        flip(t[v << 1], tl, tm);
        flip(t[v << 1 | 1], tm + 1, tr);
    }
    t[v].lazy = 0;
}

void build(int v, int l, int r, vector<int> &a) {
    if (l == r) {
        t[v] = a[l];
        return;
    }
    int mid = (l + r) >> 1;
    build(v << 1, l, mid, a);
    build((v << 1) | 1, mid + 1, r, a);
    t[v] = merge(t[(v << 1)], t[(v << 1) | 1]);
}

void update(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return;
    if (l == tl && r == tr) {
        flip(t[v], tl, tr);
        return;
    }
    int tm = (tl + tr) >> 1;
    push(v, tl, tm, tr);
    update(v << 1, tl, tm, l, min(r, tm));
    update(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r);
    t[v] = merge(t[v << 1], t[v << 1 | 1]);
}

int query(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return 0;
    if (l <= tl && tr <= r)
        return t[v].heads;
    int tm = (tl + tr) >> 1;
    push(v, tl, tm, tr);
    return query(v << 1, tl, tm, l, min(r, tm)) +
           query(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r);
}
