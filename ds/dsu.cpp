struct dsu {
    int n;
    vector<int32_t> par, siz;

    dsu(int n) : n(n), par(n), siz(n) {
        for (int i = 0; i < n; ++i) {
            make_set(i);
        }
    };

    inline void make_set(int32_t v) {
        par[v] = v;
        siz[v] = 1;
    }

    int32_t find_set(int32_t v) {
        if (v == par[v]) return v;
        return par[v] = find_set(par[v]);
    }

    bool union_sets(int32_t a, int32_t b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            if (siz[a] < siz[b]) swap(a, b);
            par[b] = a;
            siz[a] += siz[b];
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

struct dsu_bipartite {

    int n;
    vector<int> rank, bipartite;
    vector<pair<int, int>> parent;

    dsu_bipartite(int n) {
        this->n = n;
        rank.assign(n, 0);
        bipartite.assign(n, 1);
        parent.reserve(n);
        for (int i = 0; i < n; ++i)
            parent.emplace_back(i, 0);
    }

    pair<int, int> find_set(int v) {
        if (v != parent[v].first) {
            int parity = parent[v].second;
            parent[v] = find_set(parent[v].first);
            parent[v].second ^= parity;
        }
        return parent[v];
    }

    void add_edge(int a, int b) {
        int x, y;
        tie(a, x) = find_set(a);
        tie(b, y) = find_set(b);
        if (a == b) {
            if (x == y) bipartite[a] = false;
        } else {
            if (rank[a] < rank[b]) swap(a, b);
            parent[b] = make_pair(a, x ^ y ^ 1);
            bipartite[a] &= bipartite[b];
            if (rank[a] == rank[b]) ++rank[a];
        }
    }

    bool is_bipartite(int v) { return bipartite[find_set(v).first]; }
};
