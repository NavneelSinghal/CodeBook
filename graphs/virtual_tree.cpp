template <template <typename> class Container = std::vector>
struct VirtualTree {
    using graph = std::vector<Container<int>>;
    VirtualTree(const graph& G, int root = 0) : N((int)G.size()), g(G), g_vt(N), par(N), start(N), depth(N), sz(N), in_time(N) {
        par[root] = -1;
        dfs_sz(root);
        start[root] = root;
        dfs_hld(root);
    }
 
    bool is_anc(int u, int v) { return in_time[u] <= in_time[v] && in_time[u] + sz[u] > in_time[v]; }
 
    void dfs_sz(int u) {
        sz[u] = 1;
        for (auto& v : g[u]) {
            par[v] = u;
            depth[v] = depth[u] + 1;
            g[v].erase(std::find(std::begin(g[v]), std::end(g[v]), u));
            dfs_sz(v);
            sz[u] += sz[v];
            if (sz[v] > sz[g[u][0]]) std::swap(v, g[u][0]);
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
            if (depth[start[u]] > depth[start[v]]) std::swap(u, v);
        return depth[u] < depth[v] ? u : v;
    }
    int dist(int u, int v) { return depth[u] + depth[v] - 2 * depth[lca(u, v)]; }
    void build(std::span<int> vertices) {
        auto k = vertices.size();
        std::vector<int> v(vertices.begin(), vertices.end());
        std::sort(std::begin(v), std::end(v), [&](int x, int y) { return in_time[x] < in_time[y]; });
        for (std::size_t i = 1; i < k; ++i) v.push_back(lca(v[i - 1], v[i]));
        std::sort(std::begin(v), std::end(v), [&](int x, int y) { return in_time[x] < in_time[y]; });
        v.erase(std::unique(std::begin(v), std::end(v)), std::end(v));
        k = v.size();
        for (auto x : v) g_vt[x].clear();
        std::vector<int> st;
        st.push_back(v[0]);
        for (std::size_t i = 1; i < k; ++i) {
            int u = v[i];
            while (st.size() >= 2 && !is_anc(st.back(), u)) {
                g_vt[st.end()[-2]].push_back(st.back());
                st.pop_back();
            }
            st.push_back(u);
        }
        while (st.size() >= 2) {
            g_vt[st.end()[-2]].push_back(st.back());
            st.pop_back();
        }
        root_vt = st[0];
    }
    int N;
    graph g, g_vt;
    std::vector<int> par, start, depth, sz, in_time;
    int timer{};
    int root_vt{};
};
