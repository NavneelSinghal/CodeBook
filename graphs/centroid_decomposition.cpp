{
    vector<bool> removed(n);
    vector<int> sz(n);

    auto dfs_sz = [&](const auto& self, int u, int p) -> int {
        sz[u] = 1;
        for (auto v : g[u])
            if (!removed[v] && v != p) sz[u] += self(self, v, u);
        return sz[u];
    };
    auto dfs_centroid = [&](const auto& self, int u, int p, int bound) -> int {
        for (auto v : g[u])
            if (!removed[v] && v != p && sz[v] >= bound)
                return self(self, v, u, bound);
        return u;
    };

    ll ans = 0;

    auto dfs_centroid_decomp = [&](const auto& self, int u) -> void {
        int sz_u = dfs_sz(dfs_sz, u, -1);
        int centroid = dfs_centroid(dfs_centroid, u, -1, sz_u / 2);
        removed[centroid] = 1;

        {
            // work here
            // root of the tree here is `centroid`
            // whenever you consider a vertex, check removed[v]

            // example below: https://cses.fi/problemset/task/2080

            // compute number of paths of length k passing through u
            basic_string<int> cnt_at_depth{1};
            auto dfs_depth = [&](const auto& Self, int U, int P,
                                 int depth) -> void {
                if (depth > k) return;
                if (depth == (int)cnt_at_depth.size())
                    cnt_at_depth.push_back(1);
                else
                    cnt_at_depth[depth]++;
                for (auto V : g[U])
                    if (!removed[V] && V != P) Self(Self, V, U, depth + 1);
            };

            auto dfs_acc = [&](const auto& Self, int U, int P,
                               int depth) -> void {
                if (depth > k) return;
                if (k - depth < (int)cnt_at_depth.size())
                    ans += cnt_at_depth[k - depth];
                for (auto V : g[U])
                    if (!removed[V] && V != P) Self(Self, V, U, depth + 1);
            };

            for (auto v : g[centroid])
                if (!removed[v])
                    dfs_acc(dfs_acc, v, centroid, 1),
                        dfs_depth(dfs_depth, v, centroid, 1);
        }

        for (auto v : g[centroid])
            if (!removed[v]) self(self, v);
    };

    dfs_centroid_decomp(dfs_centroid_decomp, 0);
}
