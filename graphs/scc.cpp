std::tuple<std::vector<std::vector<int>>, std::vector<std::vector<int>>,
           std::vector<int>>
find_scc(const std::vector<std::vector<int>>& g, int const Base = 0) {
    std::vector<int> val, z, component;
    std::vector<std::vector<int>> scc;
    int timer = 0, total_components = 0;
    const auto dfs = [&](const auto& self, int u) -> int {
        int low = val[u] = ++timer;
        int x;
        z.push_back(u);
        for (auto v : g[u])
            if (component[v] < 0)
                low = std::min(low, val[v] ? val[v] : self(self, v));
        if (low == val[u]) {
            scc.emplace_back();
            do {
                x = z.back();
                z.pop_back();
                component[x] = total_components;
                scc.back().push_back(x);
            } while (x != u);
            total_components++;
        }
        return val[u] = low;
    };
    int n = g.size();
    val.assign(n, 0);
    component.assign(n, -1);
    timer = total_components = 0;
    for (int i = 0; i < n; ++i)
        if (component[i] < 0) dfs(dfs, i);
    std::reverse(std::begin(scc), std::end(scc));
    for (int i = 0; i < n; ++i)
        component[i] = total_components - 1 - component[i];
    std::vector<bool> seen(n);
    std::vector<std::vector<int>> condensed_graph(total_components);
    for (int i = 0; i < total_components; ++i) {
        for (auto u : scc[i])
            for (auto v : g[u])
                if (component[v] != i && !seen[component[v]])
                    condensed_graph[i].push_back(component[v]),
                        seen[component[v]] = true;
        for (auto v : condensed_graph[i]) seen[v] = false;
    }
    return {scc, condensed_graph, component};
}
