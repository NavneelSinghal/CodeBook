// this template will break for n = 1
// to speed up, use basic_string

// MergeInto : Aggregate * Value * Vertex(int) * EdgeIndex(int) -> Aggregate
// MergeInto is an accumulator used for fold_left
// This function assumes order-invariance
template <class Aggregate, class Value, class MergeInto>
auto exclusive(const std::vector<Value>& a, const Aggregate& base,
               const MergeInto& merge_into, int vertex) {
    int n = (int)std::size(a);
    std::vector<Aggregate> b(n, base);
    for (int bit = (int)std::__lg(n); bit >= 0; --bit) {
        for (int i = n - 1; i >= 0; --i) b[i] = b[i >> 1];
        int sz = n - (n & !bit);
        for (int i = 0; i < sz; ++i) {
            int index = (i >> bit) ^ 1;
            b[index] = merge_into(b[index], a[i], vertex, i);
        }
    }
    return b;
}

// MergeInto : Aggregate * Value * Vertex(int) * EdgeIndex(int) -> Aggregate
// Base : Vertex(int) -> Aggregate
// FinalizeMerge : Aggregate * Vertex(int) -> Value

// MergeInto -> merge the value corresponding to a child into the aggregate
// FinalizeMerge -> finalize the merge (update the vertex with child aggregates)
template <class Aggregate, class Value, class MergeInto, class FinalizeMerge,
          class Base>
auto rerooter(const std::vector<std::vector<int>>& g, const Value& default_val,
              const Aggregate&, const Base& base, const MergeInto& merge_into,
              const FinalizeMerge& finalize_merge) {
    int n = (int)std::size(g);

    std::vector<Value> root_dp(n, default_val);
    std::vector<std::vector<Value>> edge_dp(n);

    std::vector<Value> dp(n, default_val);

    std::vector<int> bfs(n), parent(n, -1);
    int cnt = 1;
    parent[0] = 0;
    for (int i = 0; i < n; ++i) {
        int u = bfs[i];
        for (auto v : g[u]) {
            if (parent[v] != -1) continue;
            parent[v] = u;
            bfs[cnt++] = v;
        }
    }

    for (int i = n - 1; i >= 0; --i) {
        int u = bfs[i];
        int edge_index = -1;
        int p = parent[u];
        Aggregate aggregate = base(u);
        for (auto v : g[u]) {
            ++edge_index;
            if (v == p) continue;
            aggregate = merge_into(aggregate, dp[v], u, edge_index);
        }
        dp[u] = finalize_merge(aggregate, u);
    }

    for (auto u : bfs) {
        dp[parent[u]] = dp[u];
        edge_dp[u].reserve(g[u].size());
        for (auto v : g[u]) edge_dp[u].push_back(dp[v]);
        auto dp_exclusive = exclusive(edge_dp[u], base(u), merge_into, u);
        for (int i = 0; i < (int)dp_exclusive.size(); ++i) {
            auto v = g[u][i];
            dp[v] = finalize_merge(dp_exclusive[i], u);
        }
        root_dp[u] =
            finalize_merge(merge_into(dp_exclusive[0], edge_dp[u][0], u, 0), u);
    }

    return make_pair(root_dp, edge_dp);
}

/* sample usage
int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int n;
    cin >> n;
    std::vector<int> color(n);
    for (auto& x : color) cin >> x;
    std::vector<std::vector<int>> g(n);
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    auto merge_into = [](int vertex_dp, int neighbor_dp, int vertex,
                         int edge_index) {
        return vertex_dp + max(0, neighbor_dp);
    };
    auto finalize_merge = [&color](int vertex_dp, int vertex) {
        return vertex_dp + 2 * color[vertex] - 1;
    };
    auto base = [](int) {
        return 0;
    };
    auto [root_dp, edge_dp] =
        rerooter(g, 0, 0, base, merge_into, finalize_merge);
    for (auto& x : root_dp) cout << x << ' ';
    cout << '\n';
}
*/
