struct dsu {
    int n;
    vector<int32_t> par;

    // negative - size
    dsu(int n) : n(n), par(n, -1) {}

    int32_t find_set(int32_t v) {
        if (par[v] < 0) return v;
        return par[v] = find_set(par[v]);
    }

    bool union_sets(int32_t a, int32_t b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            if (par[b] < par[a]) swap(a, b);
            par[a] += par[b];
            par[b] = a;
            return true;
        }
        return false;
    }

    vector<vector<int>> components() {
        vector<int> representative(n), component_size(n);
        for (int i = 0; i < n; ++i) {
            representative[i] = find_set(i);
            component_size[representative[i]]++;
        }
        vector<vector<int>> components(n);
        for (int i = 0; i < n; ++i) components[i].reserve(component_size[i]);
        for (int i = 0; i < n; ++i) components[representative[i]].push_back(i);
        components.erase(
            remove_if(components.begin(), components.end(),
                      [](const vector<int>& v) { return v.empty(); }),
            components.end());
        return components;
    }
};

