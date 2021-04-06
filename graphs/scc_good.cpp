// https://github.com/cheran-senthil/PyRival/blob/7f55058bd657f52c8eda69a7332fb3db30bbaa86/pyrival/graphs/scc.py
// python translation
// https://en.wikipedia.org/wiki/Path-based_strong_component_algorithm

auto FindSCC(const vector<vector<int>> &G, int const Base) {
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
