// implementation: Narut Sereewattanawoot 
auto FindSCC(const vector<vector<int>> &G, int const Base = 0) {
    vector<vector<int>> SCC;
    vector<unsigned> S, P, depth(G.size());
    const auto dfs = [&](const auto &self, auto u) -> void {
        auto d = S.size();
        S.push_back(u);
        P.push_back(d + 1);
        depth[u] = d + 1;
        for (auto v : G[u]) {
            if (!depth[v])
                self(self, v);
            else
                while (P.back() > depth[v]) P.pop_back();
        }
        if (P.back() > d) {
            SCC.emplace_back(S.begin() + d, S.end());
            for (auto v : SCC.back()) depth[v] = -1;
            S.erase(S.begin() + d, S.end());
            P.pop_back();
        }
    };
    for (auto u = Base; u < G.size(); ++u)
        if (!depth[u]) dfs(dfs, u);
    // reverse(SCC.begin(), SCC.end());
    return SCC;
}
