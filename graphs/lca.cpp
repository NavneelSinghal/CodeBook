// implementation
// assume graph is a weighted graph
// if not, remove cost
{
    const int root = 0;
    const int LOG = __lg(n) + 2;
    int timer = 0;
    vector<int> tin(n), tout(n);
    vector<vector<int>> parent(LOG, vector<int>(n));
    y_combinator([&](const auto dfs, int u, int p) -> void {
        tin[u] = ++timer;
        parent[0][u] = p;
        for (int i = 1; i < LOG; ++i)
            parent[i][u] = parent[i - 1][parent[i - 1][u]];
        for (auto &[v, cost] : g[u])
            if (v != p) dfs(v, u);
        tout[u] = ++timer;
    })(root, root);
    auto is_ancestor = [&](int u, int v) {
        return tin[u] <= tin[v] && tout[u] >= tin[v];
    };
    auto lca = [&](int u, int v) {
        if (is_ancestor(u, v)) return u;
        if (is_ancestor(v, u)) return v;
        for (int i = LOG - 1; i >= 0; --i)
            if (!is_ancestor(parent[i][u], v)) u = parent[i][u];
        return parent[0][u];
    };
}

// another implementation
// assume graph is a weighted graph
// if not, remove cost
{
    const int root = 0;
    const int LOG = __lg(n) + 2;
    vector<vector<int>> parent(LOG, vector<int>(n));
    vector<int> depth(n);
    y_combinator([&](const auto dfs, int u, int p, int cur_depth) -> void {
        depth[u] = cur_depth;
        parent[0][u] = p;
        for (auto &[v, cost] : g[u])
            if (v != p) dfs(v, u, cur_depth + 1);
    })(root, root, 0);
    for (int i = 1; i < LOG; ++i)
        for (int j = 0; j < n; ++j)
            parent[i][j] = parent[i - 1][parent[i - 1][j]];
    auto lca = [&](int x, int y) {
        for (int i = LOG - 1; i >= 0; --i)
            if (depth[parent[i][x]] >= depth[y]) x = parent[i][x];
        for (int i = LOG - 1; i >= 0; --i)
            if (depth[parent[i][y]] >= depth[x]) y = parent[i][y];
        for (int i = LOG - 1; i >= 0; --i)
            if (parent[i][x] != parent[i][y]) {
                x = parent[i][x];
                y = parent[i][y];
            }
        if (x == y) return x;
        return parent[0][x];
    };
}

struct HLD_LCA {
    using graph = std::vector<std::vector<int>>;
    HLD_LCA(const graph &G, int root = 0)
        : N((int)G.size()),
          g(G),
          par(N),
          start(N),
          depth(N),
          sz(N),
          in_time(N) {
        par[root] = -1;
        timer = 0;
        dfs_sz(root);
        start[root] = root;
        dfs_hld(root);
    }

    bool is_anc(int u, int v) {
        return in_time[u] <= in_time[v] && in_time[u] + sz[u] >= in_time[v];
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

    int N;
    graph g;
    std::vector<int> par, start, depth, sz, in_time;
    int timer;
};

