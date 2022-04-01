template <class G>
auto block_cut_tree(const G& g) {
    int n = (int)g.size();
    std::vector<int> tin(n, -1), low(n, -1), is_cutpoint(n), stk;
    G components;
    int cutpoints = 0;
    int timer = 0;
    auto dfs = [&](auto self, int u, int p) -> void {
        tin[u] = low[u] = timer++;
        stk.push_back(u);
        for (int v : g[u]) {
            if (v == p) continue;
            if (tin[v] == -1) {
                self(self, v, u);
                low[u] = std::min(low[u], low[v]);
                if (low[v] >= tin[u]) {
                    is_cutpoint[u] = (tin[u] > 0 || tin[v] > 1);
                    typename remove_cref_<decltype(g[0])>::type component{u};
                    while (component.back() != v)
                        component.push_back(stk.back()), stk.pop_back();
                    components.push_back(component);
                }
            } else
                low[u] = std::min(low[u], tin[v]);
        }
        if (is_cutpoint[u]) cutpoints++;
    };
    for (int u = 0; u < n; ++u)
        if (tin[u] == -1) dfs(dfs, u, -1);
    G forest;
    forest.reserve(cutpoints + components.size());
    std::vector<int> id(n, -1);
    for (int u = 0; u < n; ++u)
        if (is_cutpoint[u]) id[u] = (int)forest.size(), forest.emplace_back();
    for (auto& comp : components) {
        int node = (int)forest.size();
        forest.emplace_back();
        for (int u : comp)
            if (!is_cutpoint[u])
                id[u] = node;
            else
                forest[node].push_back(id[u]), forest[id[u]].push_back(node);
    }

    // id[i] = node in the block-cut forest corresponding to vertex i

    // components[i] = ith component (components also store cutpoints in them)

    // is_cutpoint[i] = 1 if i is a cutpoint, 0 otherwise

    // cutpoints = number of cutpoints (all nodes in block-cut forest numbered <
    // cutpoints are cutpoints)

    // forest = adjacency list for block-cut forest

    // one node for every cutpoint and one node for every component

    return std::tuple{forest, components, id, is_cutpoint, cutpoints};
}

