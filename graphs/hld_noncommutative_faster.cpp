using ll = long long;
constexpr int inf = 2e9;
// in general, you need to keep two sets of information - one for the segment
// and the other for the reverse of the segment
struct node_t {
    int l, r, sum, left, right, best;
};
 
using base_t = int;
using update_t = int;
 
inline node_t combine(const node_t &n1, const node_t &n2) {
    if (n1.l == -1) return n2;
    if (n2.l == -1) return n1;
    return {n1.l,
            n2.r,
            n1.sum + n2.sum,
            max(n1.left, n1.sum + n2.left),
            max(n2.right, n2.sum + n1.right),
            max({n1.best, n2.best, n1.right + n2.left})};
}
 
// to combine stuff from the downward and upward paths to the lca
// n1 is the answer to u->l, and n2 is the answer to l->v
inline node_t combine_different(const node_t &n1, const node_t &n2) {
    auto n3 = n1;
    swap(n3.left, n3.right);
    return combine(n3, n2);
}
 
inline node_t make_node(const base_t &val, int i) {
    int better = max(val, 0);
    return {i, i + 1, val, better, better, better};
}
 
inline node_t id_node() {
    return {-1, -1, 0, -inf, -inf, -inf};
}
 
inline node_t apply_update(const update_t &u, const node_t &nd) {
    if (u == -inf) return nd;
    node_t ret;
    ret.l = nd.l;
    ret.r = nd.r;
    int better = max(u, 0);
    ret.sum = u * (nd.r - nd.l);
    ret.left = better * (nd.r - nd.l);
    ret.right = ret.left;
    ret.best = ret.left;
    return ret;
}
 
inline update_t compose_updates(const update_t &u, const update_t &v) {
    if (u == -inf) return v;
    return u;
}
 
inline update_t id_update() {
    return -inf;
}
 
inline base_t init_base() {
    return 0;
}
 
template <class base_t, class node_t, class update_t,
          node_t (*make_node)(const base_t &, int),
          node_t (*combine)(const node_t &, const node_t &),
          node_t (*id_node)(),
          node_t (*apply_update)(const update_t &, const node_t &),
          update_t (*compose_updates)(const update_t &, const update_t &),
          update_t (*id_update)(), bool is_lazy>
struct lazy_segtree {
    using S = node_t;
    using F = update_t;
    using B = base_t;
 
   public:
    lazy_segtree() : lazy_segtree(0) {}
    explicit lazy_segtree(int n)
        : lazy_segtree(std::vector<B>(n, init_base())) {}
    explicit lazy_segtree(const std::vector<B> &v) : _n(int(v.size())) {
        log = 0;
        while ((1 << log) < _n) ++log;
        size = 1 << log;
        d = std::vector<S>(2 * size, id_node());
        if constexpr (is_lazy) lz = std::vector<F>(size, id_update());
        for (int i = 0; i < _n; i++) d[size + i] = make_node(v[i], i);
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }
    void set(int p, S x) {
        p += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    S get(int p) {
        p += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }
    S query(int l, int r) {
        if (l == r) return id_node();
        l += size, r += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) {
                if (((l >> i) << i) != l) push(l >> i);
                if (((r >> i) << i) != r) push((r - 1) >> i);
            }
        S sml = id_node(), smr = id_node();
        while (l < r) {
            if (l & 1) sml = combine(sml, d[l++]);
            if (r & 1) smr = combine(d[--r], smr);
            l >>= 1, r >>= 1;
        }
        return combine(sml, smr);
    }
    S all_query() { return d[1]; }
    void update(int p, F f) {
        p += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = apply_update(f, d[p]);
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    void update(int l, int r, F f) {
        if (l == r) return;
        l += size, r += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) {
                if (((l >> i) << i) != l) push(l >> i);
                if (((r >> i) << i) != r) push((r - 1) >> i);
            }
        {
            int l2 = l, r2 = r;
            while (l < r) {
                if (l & 1) all_apply(l++, f);
                if (r & 1) all_apply(--r, f);
                l >>= 1, r >>= 1;
            }
            l = l2, r = r2;
        }
        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }
    template <bool (*g)(S)>
    int max_right(int l) {
        return max_right(l, [](S x) { return g(x); });
    }
    template <class G>
    int max_right(int l, G g) {
        if (l == _n) return _n;
        l += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(l >> i);
        S sm = id_node();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(combine(sm, d[l]))) {
                while (l < size) {
                    if constexpr (is_lazy) push(l);
                    l = (2 * l);
                    if (g(combine(sm, d[l]))) {
                        sm = combine(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = combine(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }
    template <bool (*g)(S)>
    int min_left(int r) {
        return min_left(r, [](S x) { return g(x); });
    }
    template <class G>
    int min_left(int r, G g) {
        if (r == 0) return 0;
        r += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push((r - 1) >> i);
        S sm = id_node();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(combine(d[r], sm))) {
                while (r < size) {
                    if constexpr (is_lazy) push(r);
                    r = (2 * r + 1);
                    if (g(combine(d[r], sm))) {
                        sm = combine(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = combine(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
 
   private:
    int _n, size, log;
    std::vector<S> d;
    std::vector<F> lz;
    void update(int k) { d[k] = combine(d[2 * k], d[2 * k + 1]); }
    void all_apply(int k, F f) {
        d[k] = apply_update(f, d[k]);
        if constexpr (is_lazy)
            if (k < size) lz[k] = compose_updates(f, lz[k]);
    }
    void push(int k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        if constexpr (is_lazy) lz[k] = id_update();
    }
};
template <bool is_lazy>
using lazy_seg =
    lazy_segtree<base_t, node_t, update_t, make_node, combine, id_node,
                 apply_update, compose_updates, id_update, is_lazy>;
template <bool vals_in_edges, class base_t, class node_t, class update_t,
          node_t (*make_node)(const base_t &, int),
          node_t (*combine)(const node_t &, const node_t &),
          node_t (*combine_different)(const node_t &, const node_t &),
          node_t (*id_node)(),
          node_t (*apply_update)(const update_t &, const node_t &),
          update_t (*compose_updates)(const update_t &, const update_t &),
          update_t (*id_update)(), bool is_lazy>
struct NonCommutativeHLD {
    using graph = std::vector<std::vector<int>>;
    NonCommutativeHLD(const graph &G, int root = 0)
        : N((int)G.size()),
          g(G),
          par(N),
          start(N),
          depth(N),
          sz(N),
          in_time(N),
          tree(N) {
        par[root] = -1;
        timer = 0;
        dfs_sz(root);
        start[root] = root;
        dfs_hld(root);
    }
 
    bool is_anc(int u, int v) {
        return in_time[u] <= in_time[v] && in_time[u] + sz[u] >= in_time[v];
    }
 
    void update_path(int u, int v, update_t val) {
        // should customize val if breaking into segments changes val
        int l = lca(u, v);
        if (in_time[u] > in_time[v]) swap(u, v);
        // u -> l, l -> v
        while (u != -1 && is_anc(l, u)) {
            int ql = max(in_time[start[u]], in_time[l] + vals_in_edges);
            int qr = in_time[u] + 1;
            tree.update(ql, qr, val);
            u = par[start[u]];
        }
        while (v != -1 && is_anc(l, v)) {
            int ql = max(in_time[start[v]],
                         in_time[l] + 1);  // to avoid overcounting
            int qr = in_time[v] + 1;
            tree.update(ql, qr, val);
            v = par[start[v]];
        }
    }
    node_t query_path(int u, int v) {
        const int l = lca(u, v);
        if (in_time[u] > in_time[v]) swap(u, v);
        // u -> l, l -> v
        node_t res_l = id_node(), res_r = id_node();
        while (u != -1 && is_anc(l, u)) {
            int ql = max(in_time[start[u]], in_time[l] + vals_in_edges);
            int qr = in_time[u] + 1;
            res_l = combine(tree.query(ql, qr), res_l);
            u = par[start[u]];
        }
        while (v != -1 && is_anc(l, v)) {
            int ql = max(in_time[start[v]],
                         in_time[l] + 1);  // to avoid overcounting
            int qr = in_time[v] + 1;
            res_r = combine(tree.query(ql, qr), res_r);
            v = par[start[v]];
        }
        // these nodes have different orientation
        return combine_different(res_l, res_r);
    }
    node_t query_vertex(int u) {
        return tree.query(in_time[u], in_time[u] + 1);
    }
    void update_vertex(int u, update_t val) {
        int l = in_time[u];
        int r = l + 1;
        tree.update(l, r, val);
    }
    // subtree updates and queries might not even make sense
    void update_subtree(int u, node_t val) {
        int l = in_time[u] + vals_in_edges;
        int r = in_time[u] + sz[u];
        tree.update(l, r, val);
    }
    // same here
    node_t query_subtree(int u) {
        return tree.query(in_time[u] + vals_in_edges, in_time[u] + sz[u]);
    }
    void dfs_sz(int u) {
        sz[u] = 1;
        for (auto &v : g[u]) {
            par[v] = u;
            depth[v] = depth[u] + 1;
            g[v].erase(find(begin(g[v]), end(g[v]), u));
            dfs_sz(v);
            sz[u] += sz[v];
            if (sz[v] > sz[g[u][0]]) swap(v, g[u][0]);
        }
    }
    void dfs_hld(int u) {
        in_time[u] = timer++;
        for (auto &v : g[u]) {
            start[v] = (v == g[u][0] ? start[u] : v);
            dfs_hld(v);
        }
    }
    int lca(int u, int v) {
        for (; start[u] != start[v]; v = par[start[v]])
            if (depth[start[u]] > depth[start[v]]) swap(u, v);
        return depth[u] < depth[v] ? u : v;
    }
    int dist(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }
 
   private:
    int N;
    graph g;
    std::vector<int> par, start, depth, sz, in_time;
    lazy_segtree<base_t, node_t, update_t, make_node, combine, id_node,
                 apply_update, compose_updates, id_update, is_lazy>
        tree;
    int timer;
};

