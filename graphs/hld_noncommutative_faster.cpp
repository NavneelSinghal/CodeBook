// clang-format off
template <class Base,
          class Node,
          class Update,
          class MakeNode,
          class CombineNodes,
          class ApplyUpdate,
          class ComposeUpdates = std::nullptr_t>
struct lazy_segtree {
    static constexpr bool is_lazy =
        !std::is_same<ComposeUpdates, std::nullptr_t>::value;

   public:
    template <typename... T>
    explicit lazy_segtree(int n, const Base& id_base, T... args)
        : lazy_segtree(std::vector<Base>(n, id_base), args...) {}
    explicit lazy_segtree(const std::vector<Base>& v,
                          const Node& _id_node,
                          const MakeNode& _make_node,
                          const CombineNodes& _combine,
                          const Update& _id_update,
                          const ApplyUpdate& _apply_update,
                          const ComposeUpdates& _compose_updates = nullptr)
        : _n(int(v.size())),
          make_node(_make_node),
          combine(_combine),
          id_node(_id_node),
          apply_update(_apply_update),
          id_update(_id_update),
          compose_updates(_compose_updates) {
        build(v);
    }

    void build(const std::vector<Base>& v) {
        _n = int(v.size());
        log = 0;
        while ((1 << log) < _n) ++log;
        size = 1 << log;
        d = std::vector<Node>(2 * size, id_node);
        if constexpr (is_lazy) lz = std::vector<Update>(size, id_update);
        for (int i = 0; i < _n; i++) d[size + i] = make_node(v[i], i);
        for (int i = size - 1; i >= 1; i--) update(i);
    }

    void set(int p, Node x) {
        p += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    Node get(int p) {
        p += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }

    Node query(int l, int r) {
        if (l == r) return id_node;
        l += size, r += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) {
                if (((l >> i) << i) != l) push(l >> i);
                if (((r >> i) << i) != r) push((r - 1) >> i);
            }
        Node sml = id_node, smr = id_node;
        while (l < r) {
            if (l & 1) sml = combine(sml, d[l++]);
            if (r & 1) smr = combine(d[--r], smr);
            l >>= 1, r >>= 1;
        }
        return combine(sml, smr);
    }
    Node all_query() { return d[1]; }
    void update(int p, Update f) {
        p += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = apply_update(f, d[p]);
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    void update(int l, int r, Update f) {
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

    template <class G>
    int max_right(int l, G g) {
        // assert(0 <= l && l <= _n);
        // assert(g(id_node));
        if (l == _n) return _n;
        l += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(l >> i);
        Node sm = id_node;
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

    template <class G>
    int min_left(int r, G g) {
        // assert(0 <= r && r <= _n);
        // assert(g(id_node));
        if (r == 0) return 0;
        r += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push((r - 1) >> i);
        Node sm = id_node;
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
    std::vector<Node> d;
    std::vector<Update> lz;
    const MakeNode make_node;
    const CombineNodes combine;
    const Node id_node;
    const ApplyUpdate apply_update;
    const Update id_update;
    const ComposeUpdates compose_updates;

    void update(int k) { d[k] = combine(d[2 * k], d[2 * k + 1]); }
    void all_apply(int k, Update f) {
        d[k] = apply_update(f, d[k]);
        if constexpr (is_lazy)
            if (k < size) lz[k] = compose_updates(f, lz[k]);
    }
    void push(int k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        if constexpr (is_lazy) lz[k] = id_update;
    }
};

// clang-format off
template <class Base,
          class Node,
          class Update,
          class MakeNode,
          class CombineNodes,
          class CombineDifferent,
          class ApplyUpdate,
          class ComposeUpdates = std::nullptr_t>
// clang-format on
struct NonCommutativeHLD {
    using graph = std::vector<std::vector<int>>;

    static constexpr bool vals_in_edges = false;

    // clang-format off
    NonCommutativeHLD(const graph& G, int root, std::vector<Base>& vertex_vals,
                      const Base& _id_base,
                      const Node& _id_node,
                      const MakeNode& _make_node,
                      const CombineNodes& _combine,
                      const CombineDifferent& _combine_different,
                      const Update& _id_update,
                      const ApplyUpdate& _apply_update,
                      const ComposeUpdates& _compose_updates = nullptr)
        // clang-format on
        : N((int)G.size()),
          g(G),
          par(N),
          start(N),
          depth(N),
          sz(N),
          in_time(N),
          combine(_combine),
          combine_different(_combine_different),
          id_node(_id_node),
          tree(0, _id_base, _id_node, _make_node, _combine, _id_update,
               _apply_update, _compose_updates) {
        par[root] = -1;
        timer = 0;
        dfs_sz(root);
        start[root] = root;
        dfs_hld(root);
        std::vector<Base> perm(N);
        for (int i = 0; i < N; ++i) perm[in_time[i]] = vertex_vals[i];
        tree.build(perm);
    }

    bool is_anc(int u, int v) {
        return in_time[u] <= in_time[v] && in_time[u] + sz[u] >= in_time[v];
    }

    void update_path(int u, int v, Update val) {
        // should customize val if breaking into segments changes val
        int l = lca(u, v);
        // u -> l, l -> v
        while (u != -1 && is_anc(l, u)) {
            int ql = max(in_time[start[u]], in_time[l] + vals_in_edges);
            int qr = in_time[u] + 1;
            tree.update(ql, qr, val);
            u = par[start[u]];
        }
        while (v != -1 && is_anc(l, v)) {
            int ql = max(in_time[start[v]], in_time[l] + 1);
            int qr = in_time[v] + 1;
            tree.update(ql, qr, val);
            v = par[start[v]];
        }
    }
    Node query_path(int u, int v) {
        const int l = lca(u, v);
        // u -> l, l -> v
        Node res_l = id_node, res_r = id_node;
        while (u != -1 && is_anc(l, u)) {
            int ql = max(in_time[start[u]], in_time[l] + vals_in_edges);
            int qr = in_time[u] + 1;
            res_l = combine(tree.query(ql, qr), res_l);
            u = par[start[u]];
        }
        while (v != -1 && is_anc(l, v)) {
            int ql = max(in_time[start[v]], in_time[l] + 1);
            int qr = in_time[v] + 1;
            res_r = combine(tree.query(ql, qr), res_r);
            v = par[start[v]];
        }
        // these nodes have different orientation
        return combine_different(res_l, res_r);
    }
    Node query_vertex(int u) { return tree.get(in_time[u]); }
    void update_vertex(int u, Update val) { tree.set(in_time[u], val); }
    // subtree updates and queries might not even make sense
    void update_subtree(int u, Node val) {
        int l = in_time[u] + vals_in_edges;
        int r = in_time[u] + sz[u];
        tree.update(l, r, val);
    }
    Node query_subtree(int u) {
        return tree.query(in_time[u] + vals_in_edges, in_time[u] + sz[u]);
    }
    void dfs_sz(int u) {
        sz[u] = 1;
        for (auto& v : g[u]) {
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
        for (auto& v : g[u]) {
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
    const CombineNodes combine;
    const CombineDifferent combine_different;
    const Node id_node;
    lazy_segtree<Base, Node, Update, MakeNode, CombineNodes, ApplyUpdate,
                 ComposeUpdates>
        tree;
    int timer;
};

/* usage sample
    struct Node {
        ll s1, s2, s3;
        int size;
        Node() : s1(0), s2(0), s3(0), size(0) {}
        Node(ll x) : s1(0), s2(x), s3(0), size(1) {}
        Node(ll _s1, ll _s2, ll _s3, int _size)
            : s1(_s1), s2(_s2), s3(_s3), size(_size) {}
    };
    const Node id_node = Node();
    using Base = ll;
    const Base id_base = 0;
    auto make_node = [&](const Base& val, int i) {
        return Node(val);
    };
    auto combine = [&](const Node& a, const Node& b) {
        return Node{a.s1 + b.s1 + a.size * b.s2, a.s2 + b.s2,
                    a.s3 + b.s3 + b.size * a.s2, a.size + b.size};
    };
    auto combine_different = [&](const Node& n1, const Node& n2) {
        auto n3 = n1;
        swap(n3.s1, n3.s3);
        return combine(n3, n2);
    };
    using Update = ll;
    const Update id_update = -inf;
    auto apply_update = [&](const Update& u, const Node& nd) {
        if (u == -inf) return nd;
        ll s1 = 1LL * nd.size * (nd.size - 1) / 2 * u;
        ll sum = nd.size * u;
        return Node{s1, sum, s1, nd.size};
    };

    // parameters: graph, root, vertex values, id_node, make_node, combine,
    // combine_different, id_update, apply_update, compose_updates
    //
    // for edges, change static constexpr thing in class
    //
    // for lazy segtree, pass compose_updates as well
    //
    NonCommutativeHLD hld(g, 0, weight, id_base, id_node, make_node, combine,
                          combine_different, id_update, apply_update);
*/

