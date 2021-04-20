#include <bits/stdc++.h>
using namespace std;

const int INF = INT_MAX;
struct info {
    int mx1, mx2, mx_cnt, mn1, mn2, mn_cnt;

    info(int a = -INF,
         int b = -INF,
         int c = 0,
         int d = INF,
         int e = INF,
         int f = 0) {
        mx1 = a, mx2 = b, mx_cnt = c, mn1 = d, mn2 = e, mn_cnt = f;
    }
    friend inline info merge(info u, info v) {
        if (u.mx1 < v.mx1) {
            u.mx_cnt = 0;
            u.mx2 = u.mx1;
            u.mx1 = v.mx1;
        }
        if (u.mx1 == v.mx1) {
            u.mx_cnt += v.mx_cnt;
            u.mx2 = max(u.mx2, v.mx2);
        } else
            u.mx2 = max(u.mx2, v.mx1);
        if (u.mn1 > v.mn1) {
            u.mn_cnt = 0;
            u.mn2 = u.mn1;
            u.mn1 = v.mn1;
        }
        if (u.mn1 == v.mn1) {
            u.mn_cnt += v.mn_cnt;
            u.mn2 = min(u.mn2, v.mn2);
        } else
            u.mn2 = min(u.mn2, v.mn1);
        return u;
    }
};
struct node {
    info p;
    long long sum;
    int mx_lazy, mn_lazy;

    node(info a = info(), long long b = 0) { p = a, sum = b, reset(); }
    inline void reset() { mx_lazy = -INF, mn_lazy = INF; }
    inline void set_max(int x) {
        if (x <= p.mn1)
            return;
        sum += 1LL * (x - p.mn1) * p.mn_cnt;
        if (p.mx1 == p.mn1)
            p.mx1 = x;
        if (p.mx2 == p.mn1)
            p.mx2 = x;
        p.mn1 = mx_lazy = x;
    }
    inline void set_min(int x) {
        if (x >= p.mx1)
            return;
        sum += 1LL * (x - p.mx1) * p.mx_cnt;
        if (p.mn1 == p.mx1)
            p.mn1 = x;
        if (p.mn2 == p.mx1)
            p.mn2 = x;
        p.mx1 = mn_lazy = x;
    }
    friend inline node merge(node u, node v) {
        u.p = merge(u.p, v.p);
        u.sum += v.sum;
        u.reset();
        return u;
    }
};

const int N = 2e5;
node seg[N << 2];
int n, q, a[N];

inline void find(int id) {
    seg[id] = merge(seg[id << 1], seg[id << 1 | 1]);
}

inline void shift(int id) {
    for (auto p : {id << 1, id << 1 | 1}) {
        seg[p].set_max(seg[id].mx_lazy);
        seg[p].set_min(seg[id].mn_lazy);
    }
    seg[id].reset();
}

void build(int id = 1, int st = 0, int en = n) {
    if (en - st == 1) {
        info x;
        x.mx1 = a[st], x.mx_cnt = 1;
        x.mn1 = a[st], x.mn_cnt = 1;
        seg[id] = {x, a[st]};
        return;
    }
    int mid = st + en >> 1;
    build(id << 1, st, mid);
    build(id << 1 | 1, mid, en);
    find(id);
}

void update_max(int l, int r, int x, int id = 1, int st = 0, int en = n) {
    if (r <= st || en <= l || seg[id].p.mn1 >= x)
        return;
    if (l <= st && en <= r && seg[id].p.mn2 > x)
        return seg[id].set_max(x);
    shift(id);
    int mid = st + en >> 1;
    update_max(l, r, x, id << 1, st, mid);
    update_max(l, r, x, id << 1 | 1, mid, en);
    find(id);
}

void update_min(int l, int r, int x, int id = 1, int st = 0, int en = n) {
    if (r <= st || en <= l || seg[id].p.mx1 <= x)
        return;
    if (l <= st && en <= r && seg[id].p.mx2 < x)
        return seg[id].set_min(x);
    shift(id);
    int mid = st + en >> 1;
    update_min(l, r, x, id << 1, st, mid);
    update_min(l, r, x, id << 1 | 1, mid, en);
    find(id);
}

long long get(int l, int r, int id = 1, int st = 0, int en = n) {
    if (r <= st || en <= l)
        return 0;
    if (l <= st && en <= r)
        return seg[id].sum;
    shift(id);
    int mid = st + en >> 1;
    return get(l, r, id << 1, st, mid) + get(l, r, id << 1 | 1, mid, en);
}

inline void read_input() {
    cin >> n >> q;
    for (int i = 0; i < n; i++)
        cin >> a[i];
}

inline void write_output() {
    for (build(); q--;) {
        int t;
        cin >> t;
        if (t == 1) {
            int l, r, x;
            cin >> l >> r >> x;
            update_max(--l, r, x);
        }
        if (t == 2) {
            int l, r, x;
            cin >> l >> r >> x;
            update_min(--l, r, x);
        }
        if (t == 3) {
            int l, r;
            cin >> l >> r;
            cout << get(--l, r) << endl;
        }
    }
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    read_input(), write_output();
    return 0;
}
