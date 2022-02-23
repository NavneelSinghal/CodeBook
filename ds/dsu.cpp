struct dsu {
    int n;
    vector<int> par;

    // negative - size
    dsu(int n) : n(n), par(n, -1) {}

    int root(int v) {
        if (par[v] < 0) return v;
        return par[v] = root(par[v]);
    }

    bool join(int a, int b) {
        a = root(a);
        b = root(b);
        if (a != b) {
            if (par[b] < par[a]) swap(a, b);
            par[a] += par[b];
            par[b] = a;
            return true;
        }
        return false;
    }

    int size(int a) {
        a = root(a);
        return -par[a];
    }

    auto components() {
        vector<int> roots(n), size(n);
        for (int i = 0; i < n; ++i) {
            roots[i] = root(i);
            size[roots[i]]++;
        }
        vector<vector<int>> ans(n);
        for (int i = 0; i < n; ++i) ans[i].reserve(size[i]);
        for (int i = 0; i < n; ++i) ans[roots[i]].push_back(i);
        ans.erase(remove_if(ans.begin(), ans.end(),
                            [](const auto& v) { return v.empty(); }),
                  ans.end());
        return ans;
    }

    int count_components() {
        int ans = 0;
        for (int i = 0; i < n; ++i)
            if (root(i) == i) ans++;
        return ans;
    }
};

