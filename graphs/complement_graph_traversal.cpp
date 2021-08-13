// linear time implementation: https://codeforces.com/blog/entry/93801

{
    vector<vector<int>> g(n);
    for (int i = 0, u, v; i < m; ++i) {
        cin >> u >> v;
        --u, --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (auto& x : g) sort(begin(x), end(x));

    set<int> not_visited;
    for (int i = 0; i < n; ++i) not_visited.insert(i);

    vector<vector<int>> components;
    const auto dfs = [&](const auto& self, int u) {
        components.back().push_back(u);

        not_visited.erase(u);
        if (not_visited.empty()) return;
        auto it = not_visited.begin();
        while (true) {
            int v = *it;
            auto in_g = lower_bound(begin(g[u]), end(g[u]), v);
            if (in_g == end(g[u]) || *in_g != v) self(self, v);
            it = not_visited.upper_bound(v);
            if (it == not_visited.end()) break;
        }
    };

    for (int i = 0; i < n; ++i) {
        if (not_visited.find(i) != not_visited.end()) {
            components.emplace_back();
            dfs(dfs, i);
        }
    }
}
