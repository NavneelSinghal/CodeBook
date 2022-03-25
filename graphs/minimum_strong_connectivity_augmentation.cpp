auto minimum_strong_connectivity_augmentation(
    const std::vector<std::vector<int>>& g) {
    std::vector<std::pair<int, int>> edges;
    int n = (int)g.size();
    if (n <= 1) return edges;
    std::vector<int> indegree(n), outdegree(n);
    for (int i = 0; i < n; ++i)
        for (auto v : g[i]) indegree[v]++, outdegree[i]++;
    std::vector<bool> visited(n);
    auto dfs = [&](const auto& self, int u) -> int {
        visited[u] = true;
        if (outdegree[u] == 0) return u;
        for (auto v : g[u]) {
            if (visited[v]) continue;
            auto w = self(self, v);
            if (w != -1) return w;
        }
        return -1;
    };
    std::vector<int> matched_sources, matched_sinks, unmatched_sources,
        unmatched_sinks;
    for (int i = 0; i < n; ++i) {
        if (indegree[i] == 0 && !visited[i]) {
            auto sink = dfs(dfs, i);
            if (sink != -1)
                matched_sources.push_back(i), matched_sinks.push_back(sink);
            else
                unmatched_sources.push_back(i);
        }
    }
    for (int i = 0; i < n; ++i)
        if (outdegree[i] == 0 && !visited[i]) unmatched_sinks.push_back(i);
    std::vector<int> sources = matched_sources, sinks = matched_sinks;
    std::copy(std::begin(unmatched_sources), std::end(unmatched_sources),
              std::back_inserter(sources));
    std::copy(std::begin(unmatched_sinks), std::end(unmatched_sinks),
              std::back_inserter(sinks));
    edges.reserve(std::max(sources.size(), sinks.size()));
    int k = (int)std::min(sources.size(), sinks.size());
    int matching = (int)matched_sources.size();
    edges.emplace_back(sinks[0], sources[matching - 1]);
    for (int i = 1; i < matching; ++i)
        edges.emplace_back(sinks[i], sources[i - 1]);
    for (int i = matching; i < k; ++i) edges.emplace_back(sinks[i], sources[i]);
    for (int i = k; i < (int)sources.size(); ++i)
        edges.emplace_back(sinks[0], sources[i]);
    for (int i = k; i < (int)sinks.size(); ++i)
        edges.emplace_back(sinks[i], sources[0]);
    assert(edges.size() > 0);
    return edges;
}
