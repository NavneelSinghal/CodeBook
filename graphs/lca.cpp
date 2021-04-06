// n = number of vertices, l = ceil(log2(n))
int n, l;

// graph
vector<vector<int>> adj;

// current time
int timer;

// in time and out time
vector<int> tin, tout;

// sparse table on parents
vector<vector<int>> up;

void dfs(int v, int p) {
    tin[v] = ++timer;
    up[v][0] = p;
    for (int i = 1; i <= l; ++i) up[v][i] = up[up[v][i - 1]][i - 1];
    for (int u : adj[v]) {
        if (u != p) dfs(u, v);
    }
    tout[v] = ++timer;
}

// slightly more efficient for cache - replace tout[v] by tin[v] because if
// tin[v] \in [tin[u], tout[u]] then tout[v] also satisfies
bool is_ancestor(int u, int v) {
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = l; i >= 0; --i) {
        if (!is_ancestor(up[u][i], v)) u = up[u][i];
    }
    return up[u][0];
}

void preprocess(int root) {
    tin.resize(n);
    tout.resize(n);
    timer = 0;
    l = ceil(log2(n));
    up.assign(n, vector<int>(l + 1));
    dfs(root, root);
}

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
