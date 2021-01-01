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
    long long w;

    bool operator<(const edge &o) const {
        if (w != o.w) return w < o.w;
        return id < o.id;
    }
};

vector<bool> boruvka(int n, vector<edge> edges) {
    vector<bool> res(edges.size());

    while (!edges.empty()) {
        // find local minimal edges

        vector<int> minEdge(n, -1);

        for (int i = 0; i < edges.size(); i++) {
            if (minEdge[edges[i].a] < 0 ||
                edges[i] < edges[minEdge[edges[i].a]])
                minEdge[edges[i].a] = i;
            if (minEdge[edges[i].b] < 0 ||
                edges[i] < edges[minEdge[edges[i].b]])
                minEdge[edges[i].b] = i;
        }

        // build graph with local minimal edges

        vector<vector<int>> adj(n);

        for (int x : minEdge) {
            if (x < 0) continue;
            res[edges[x].id] = true;
            adj[edges[x].a].push_back(edges[x].b);
            adj[edges[x].b].push_back(edges[x].a);
        }

        // find components

        vector<int> component(n, -1);

        int components = 0;

        for (int i = 0; i < n; i++) {
            if (component[i] >= 0) continue;

            // dfs
            vector<int> s = {i};
            component[i] = components;

            while (!s.empty()) {
                int x = s.back();
                s.pop_back();
                for (int y : adj[x]) {
                    if (component[y] >= 0) continue;
                    component[y] = components;
                    s.push_back(y);
                }
            }

            components++;
        }

        // update

        n = components;

        for (int i = 0; i < edges.size();) {
            edges[i].a = component[edges[i].a];
            edges[i].b = component[edges[i].b];

            if (edges[i].a == edges[i].b) {
                swap(edges[i], edges.back());
                edges.pop_back();
            } else {
                i++;
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
