// problems at https://codeforces.com/blog/entry/81317

#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx,avx2,sse,sse2,sse3,sse4,popcnt")

#include <bits/stdc++.h>

using namespace std;
using ll = long long;

constexpr ll inf = 3e18L;

template <bool is_lazy = true>
struct LazySegTree {
    using node_t = ll;
    using base_t = ll;
    using update_t = ll;

    // combining two nodes
    inline node_t combine(const node_t &n1, const node_t &n2) const {
        return n1 + n2;
    }

    // create node from base value and index i
    inline node_t make_node(const base_t &val, int i) const { return 0; }

    // node corresponding to empty interval
    inline node_t id_node() const { return 0; }

    // apply update u to the whole node n
    inline node_t apply_update(const update_t &u, const node_t &nd) const {
        if (u == -inf) return nd;
        return u;
    }

    // effective update if v is applied to node, followed by u
    inline update_t compose_updates(const update_t &u,
                                    const update_t &v) const {
        if (u == -inf) return v;
        return u;
    }

    // identity update
    inline update_t id_update() const { return -inf; }

    std::vector<node_t> t;
    std::vector<update_t> lazy;
    int n;

    LazySegTree(int n) {
        this->n = n;
        if (this->n == 0) return;
        this->t.assign(2 * n - 1, id_node());
        if constexpr (is_lazy) this->lazy.assign(2 * n - 1, id_update());
        std::vector<base_t> a(n, 0);
        _build(0, 0, n, a);
    }

    LazySegTree(std::vector<base_t> &a) {
        this->n = (int)a.size();
        if (this->n == 0) return;
        this->t.assign(2 * a.size() - 1, id_node());
        if constexpr (is_lazy) this->lazy.assign(2 * a.size() - 1, id_update());
        _build(0, 0, n, a);
    }

    // half open
    void update(int l, int r, const update_t &u) {
        if constexpr (!is_lazy) assert(l == r - 1);
        ql = l, qr = r;
        if (l >= r) return;
        _update(0, 0, n, u);
    }
    node_t query(int l, int r) {
        ql = l, qr = r;
        if (l >= r) return id_node();
        return _query(0, 0, n);
    }

    template <bool b = is_lazy, typename F>
    int first_false_right(int l, const F &f) {
        auto acc = id_node();
        ql = l, qr = n;
        auto i = _first_false_right<b, F>(0, 0, n, f, acc);
        if (i == -1) return n;
        return i;
    }

   private:
    int ql, qr;
    // helper functions
    inline void _pullUp(int v, int l, int m) {
        t[v] = combine(t[v + 1], t[v + ((m - l) << 1)]);
    }
    inline void _updateNode(int v, const update_t &u) {
        t[v] = apply_update(u, t[v]);
        if constexpr (is_lazy) lazy[v] = compose_updates(u, lazy[v]);
    }
    inline void _pushDown(int v, int l, int m) {
        if constexpr (!is_lazy) return;
        if (lazy[v] == id_update()) return;
        _updateNode(v + 1, lazy[v]);
        _updateNode(v + ((m - l) << 1), lazy[v]);
        lazy[v] = id_update();
    }

    // actual functions
    void _build(int v, int l, int r, const std::vector<base_t> &a) {
        if (l == r - 1) {
            t[v] = make_node(a[l], l);
            return;
        }
        int m = (l + r) / 2;
        _build(v + 1, l, m, a);
        _build(v + ((m - l) << 1), m, r, a);
        _pullUp(v, l, m);
    }

    void _update(int v, int l, int r, const update_t &u) {
        if (ql <= l && r <= qr) {
            _updateNode(v, u);
            return;
        }
        int m = (l + r) / 2;
        _pushDown(v, l, m);
        if (ql < m) _update(v + 1, l, m, u);
        if (m < qr) _update(v + ((m - l) << 1), m, r, u);
        _pullUp(v, l, m);
    }

    node_t _query(int v, int l, int r) {
        if (ql <= l && r <= qr) return t[v];
        int m = (l + r) / 2;
        _pushDown(v, l, m);
        if (m >= qr) return _query(v + 1, l, m);
        if (m <= ql) return _query(v + ((m - l) << 1), m, r);
        return combine(_query(v + 1, l, m), _query(v + ((m - l) << 1), m, r));
    }

    template <bool b = true, typename F>
    int _first_false_right(int v, int l, int r, const F &f, node_t &acc) {
        if (r <= ql) return -1;
        if (qr <= l) return l;
        auto new_acc = combine(acc, t[v]);
        if (ql <= l && r <= qr && f(new_acc)) {
            acc = new_acc;
            return -1;
        }
        if (l == r - 1) return l;
        int m = (r + l) / 2;
        if constexpr (b) _pushDown(v, l, m);
        if (ql < m) {
            auto res = _first_false_right<b, F>(v + 1, l, m, f, acc);
            if (res != -1) return res;
        }
        if (m < qr)
            return _first_false_right<b, F>(v + ((m - l) << 1), m, r, f, acc);
        return -1;
    }
};

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int t = 1;
    // cin >> t;

    while (t--) {
        int n, q;
        cin >> n >> q;
        vector<vector<int>> g(n);
        vector<ll> v(n);
        for (auto &&_x : v) cin >> _x;
        for (int i = 1; i < n; ++i) {
            int u, v;
            cin >> u >> v;
            --u, --v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        vector<int> sz(n), start(n), in_time(n), out_time(n), par(n, -1);
        iota(begin(start), end(start), 0);
        // after this, g[u][0] contains the child with the largest subtree
        auto dfs_sz = [&](const auto &self, int u, int p) -> void {
            sz[u] = 1;
            par[u] = p;
            for (auto &v : g[u]) {
                if (v == p) continue;
                self(self, v, u);
                sz[u] += sz[v];
                if (sz[v] > sz[g[u][0]]) swap(v, g[u][0]);
            }
        };
        dfs_sz(dfs_sz, 0, -1);
        // after this, start[v] = first vertex of the chain of which v is a part
        auto dfs_hld = [&](const auto &self, int u, int p,
                           int timer = 0) -> int {
            in_time[u] = timer++;
            for (auto v : g[u]) {
                if (v == p) continue;
                start[v] = (v == g[u][0] ? start[u] : v);
                timer = self(self, v, u, timer);
            }
            return out_time[u] = timer;
        };
        dfs_hld(dfs_hld, 0, -1);
        // now [in_time[v], out_time[v]) corresponds to subtree of v,
        // and [in_time[start[v]], in_time[v]] corresponds to path from v to
        // start of chain, since the heavy path always is the first vertex
        // to be explored
        LazySegTree<false> seg(n);
        for (int i = 0; i < n; ++i)
            seg.update(in_time[i], in_time[i] + 1, v[i]);
        for (int _q = 0; _q < q; ++_q) {
            int typ, u;
            cin >> typ >> u;
            --u;
            if (typ == 1) {
                ll x;
                cin >> x;
                seg.update(in_time[u], in_time[u] + 1, x);
            } else {
                ll ans = 0;
                while (u != -1) {
                    ans += seg.query(in_time[start[u]], in_time[u] + 1);
                    u = par[start[u]];
                }
                cout << ans << '\n';
            }
        }
    }
}
