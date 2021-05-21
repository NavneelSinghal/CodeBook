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

