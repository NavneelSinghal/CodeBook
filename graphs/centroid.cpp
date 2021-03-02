int OneCentroid(int root, const vector<vector<int>> &g,
                const vector<bool> &dead) {
    static vector<int> sz(g.size());
    function<void(int, int)> get_sz = [&](int u, int prev) {
        sz[u] = 1;
        for (auto v : g[u])
            if (v != prev && !dead[v]) {
                get_sz(v, u);
                sz[u] += sz[v];
            }
    };
    get_sz(root, -1);
    int n = sz[root];
    function<int(int, int)> dfs = [&](int u, int prev) {
        for (auto v : g[u])
            if (v != prev && !dead[v]) {
                if (sz[v] > n / 2) {
                    return dfs(v, u);
                }
            }
        return u;
    };
    return dfs(root, -1);
}

void CentroidDecomposition(const vector<vector<int>> &g) {
    int n = (int)g.size();
    vector<bool> dead(n, false);
    function<void(int)> rec = [&](int start) {
        int c = OneCentroid(start, g, dead);  // 2
        dead[c] = true;                       // 2
        for (auto u : g[c])
            if (!dead[u]) {
                rec(u);  // 3
            }
        /*
                compute something with the centroid    //4
        */
        dead[c] = false;  // 5
    };
    rec(0);  // 1
}

int OneCentroid(int root, const vector<vector<int>> &g,
                const vector<bool> &dead) {
    static vector<int> sz(g.size());
    function<void(int, int)> get_sz = [&](int u, int prev) {
        sz[u] = 1;
        for (auto v : g[u])
            if (v != prev && !dead[v]) {
                get_sz(v, u);
                sz[u] += sz[v];
            }
    };
    get_sz(root, -1);
    int n = sz[root];
    function<int(int, int)> dfs = [&](int u, int prev) {
        for (auto v : g[u])
            if (v != prev && !dead[v]) {
                if (sz[v] > n / 2) {
                    return dfs(v, u);
                }
            }
        return u;
    };
    return dfs(root, -1);
}

// example - https://codeforces.com/blog/entry/58025

int OneCentroid(int root, const vector<vector<int>> &g,
                const vector<bool> &dead) {
    static vector<int> sz(g.size());
    function<void(int, int)> get_sz = [&](int u, int prev) {
        sz[u] = 1;
        for (auto v : g[u])
            if (v != prev && !dead[v]) {
                get_sz(v, u);
                sz[u] += sz[v];
            }
    };
    get_sz(root, -1);
    int n = sz[root];
    function<int(int, int)> dfs = [&](int u, int prev) {
        for (auto v : g[u])
            if (v != prev && !dead[v]) {
                if (sz[v] > n / 2) {
                    return dfs(v, u);
                }
            }
        return u;
    };
    return dfs(root, -1);
}

vector<int> CentroidDecomposition(const vector<vector<int>> &g,
                                  const vector<vector<pair<int, int>>> &l,
                                  int q) {
    int n = (int)g.size();
    vector<int> ans(q, 0);
    vector<bool> dead(n, false);
    function<void(int)> rec = [&](int start) {
        int c = OneCentroid(start, g, dead);
        dead[c] = true;
        for (auto u : g[c])
            if (!dead[u]) {
                rec(u);
            }

        /*
                changed from here
        */
        map<int, int> cnt;
        function<void(int, int, int, bool)> add_cnt = [&](int u, int prev,
                                                          int d, bool add) {
            cnt[d] += (add ? 1 : -1);
            for (auto v : g[u])
                if (v != prev && !dead[v]) {
                    add_cnt(v, u, d + 1, add);
                }
        };
        function<void(int, int, int)> calc = [&](int u, int prev, int d) {
            for (auto it : l[u]) {
                int dd, idx;
                tie(dd, idx) = it;
                if (dd - d >= 0 && cnt.count(dd - d)) {
                    ans[idx] += cnt[dd - d];
                }
            }
            for (auto v : g[u])
                if (v != prev && !dead[v]) {
                    calc(v, u, d + 1);
                }
        };
        add_cnt(c, -1, 0, true);
        for (auto it : l[c]) {
            int dd, idx;
            tie(dd, idx) = it;
            ans[idx] += cnt[dd];
        }
        for (auto u : g[c])
            if (!dead[u]) {
                add_cnt(u, c, 1, false);
                calc(u, c, 1);
                add_cnt(u, c, 1, true);
            }

        dead[c] = false;
    };
    rec(0);
    return ans;
}

// for more probs: https://codeforces.com/blog/entry/81661
