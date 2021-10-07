#pragma GCC optimize("Ofast")
#pragma GCC target("avx")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>

using namespace std;

void setIO(string name = "") {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    if (name.size() == 0) return;
    FILE *fin = freopen((name + ".in").c_str(), "r", stdin);
    FILE *fout = freopen((name + ".out").c_str(), "w", stdout);
    cout << setprecision(10) << fixed;
}

struct edge {
    int a, b, id;
    ll w;
    bool operator<(const edge& o) const {
        if (w != o.w) return w < o.w;
        return id < o.id;
    }
};

auto boruvka(int n, vector<edge> edges) {
    vector<bool> res(edges.size());
    while (!edges.empty()) {
        // find local minimal edges
        vector<int> min_edge(n, -1);
        int m = (int)edges.size();
        for (int i = 0; i < m; ++i) {
            auto [u, v, _, __] = edges[i];
            auto& e_u = min_edge[u];
            auto& e_v = min_edge[v];
            if (e_u < 0 || edges[i] < edges[e_u]) e_u = i;
            if (e_v < 0 || edges[i] < edges[e_v]) e_v = i;
        }
        // build graph with local minimal edges
        vector<basic_string<int>> g(n);
        for (int x : min_edge) {
            if (x == -1) continue;
            auto [u, v, id, _] = edges[x];
            if (res[id]) continue;
            res[id] = true;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        // find components
        vector<int> component(n, -1);
        int components = 0;
        basic_string<int> s;
        for (int i = 0; i < n; ++i) {
            if (component[i] != -1) continue;
            s.push_back(i);
            component[i] = components;
            while (!s.empty()) {
                int u = s.back();
                s.pop_back();
                for (int v : g[u]) {
                    if (component[v] != -1) continue;
                    component[v] = components;
                    s.push_back(v);
                }
            }
            ++components;
        }
        // update
        n = components;
        for (int i = 0; i < (int)edges.size();) {
            auto& [a, b, _, __] = edges[i];
            a = component[a];
            b = component[b];
            if (a == b) {
                swap(edges[i], edges.back());
                edges.pop_back();
            } else {
                ++i;
            }
        }
    }
    return res;
}

void solve(int case_no) {
    long long ans = 0;

    int n, m;
    cin >> n >> m;

    vector<edge> g(m);
    for (int i = 0; i < m; ++i) {
        cin >> g[i].a >> g[i].b >> g[i].w;
        g[i].id = i;
        --g[i].a, --g[i].b;
    }

    auto vbool = boruvka(n, g);

    for (int i = 0; i < m; ++i) {
        if (vbool[i]) ans += g[i].w;
    }

    cout << ans << '\n';
}

signed main() {
    setIO();
    int t = 1;
    // cin >> t;
    for (int _t = 1; _t <= t; _t++) {
        solve(_t);
    }
    return 0;
}
