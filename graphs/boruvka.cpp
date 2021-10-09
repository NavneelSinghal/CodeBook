struct dsu {
    int n;
    vector<int> par;
    dsu(int _n) : n(_n), par(n, -1) {}
    int find_set(int v) {
        if (par[v] < 0) return v;
        return par[v] = find_set(par[v]);
    }
    bool union_sets(int a, int b) {
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
};

struct edge {
    int a, b;
    ll w;
};

auto boruvka(const int n, const vector<edge>& edges) {
    vector<bool> res(edges.size());
    ll ans = 0;
    if (n == 1) return make_pair(res, ans);
    vector<int> min_edge(n, -1);
    dsu dsu(n);
    int n_components = n;
    while (true) {
        {
            int i = 0;
            for (auto [u, v, w] : edges) {
                if (res[i]) {
                    ++i;
                    continue;
                }
                int ru = dsu.find_set(u);
                int rv = dsu.find_set(v);
                if (ru == rv) {
                    ++i;
                    continue;
                }
                auto& eu = min_edge[ru];
                auto& ev = min_edge[rv];
                if (eu == -1 || w < edges[eu].w) eu = i;
                if (ev == -1 || w < edges[ev].w) ev = i;
                ++i;
            }
        }
        for (int i = 0; i < n; ++i) {
            auto e = min_edge[i];
            if (e == -1 || res[e]) continue;
            auto [u, v, w] = edges[e];
            if (dsu.union_sets(u, v)) {
                ans += w;
                --n_components;
                res[e] = true;
            }
        }
        if (n_components > 1)
            fill(begin(min_edge), end(min_edge), -1);
        else
            break;
    }
    return make_pair(res, ans);
}
