// returns list of components

vector<vector<int>> biconnected_components(vector<vector<int>> &adj) {
    int n = adj.size();
    vector<int> tin(n), low(n), art(n), stk;
    vector<vector<int>> comps;

    function<void(int, int, int &)> dfs = [&](int u, int p, int &t) {
        tin[u] = low[u] = ++t;
        stk.push_back(u);
        for (int v : adj[u]) {
            if (v == p) continue;
            if (!tin[v]) {
                dfs(v, u, t);
                low[u] = min(low[u], low[v]);
                if (low[v] >= tin[u]) {
                    art[u] = (tin[u] > 1 || tin[v] > 2);
                    comps.push_back({u});
                    while (comps.back().back() != v) {
                        comps.back().push_back(stk.back());
                        stk.pop_back();
                    }
                }
            } else {
                low[u] = min(low[u], tin[v]);
            }
        }
    };

    for (int u = 0, t; u < n; ++u) {
        if (!tin[u]) {
            dfs(u, -1, t = 0);
        }
    }

    // build the block cut tree
    function<vector<vector<int>>()> build_tree = [&]() {
        vector<vector<int>> tree;
        vector<int> id(n);
        for (int u = 0; u < n; ++u) {
            if (art[u]) {
                id[u] = tree.size();
                tree.push_back({});
            }
        }
        for (auto &comp : comps) {
            int node = tree.size();
            tree.push_back({});
            for (int u : comp) {
                if (!art[u]) {
                    id[u] = node;
                } else {
                    tree[node].push_back(id[u]);
                    tree[id[u]].push_back(node);
                }
            }
        }
        return tree;
    };

    return comps;
}
