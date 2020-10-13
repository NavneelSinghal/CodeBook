#pragma GCC optimize("Ofast")
#pragma GCC target("avx")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>

using namespace std;

void setIO(string name = "") {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    if (name.size() == 0)
        return;
    FILE *fin = freopen((name + ".in").c_str(), "r", stdin);
    FILE *fout = freopen((name + ".out").c_str(), "w", stdout);
    cout << setprecision(10) << fixed;
}

const int N = (int)1e5 + 7;
const int M = N;

struct edge {
    int u, v, cost;
};

int n, m;

int par[N], siz[N];

int min_edge[N];
edge g[M];

void init_dsu() {
    for (int i = 1; i <= n; ++i) {
        par[i] = i;
        siz[i] = 1;
    }
}

int root(int v) {
    if (par[v] == v) return v;
    return par[v] = root(par[v]);
}

bool merge(int v, int u) {
    v = root(v);
    u = root(u);
    if (v == u) return false;
    if (siz[u] < siz[v]) swap(u, v);
    par[v] = u;
    siz[u] += siz[v];
    return true;
}

void solve(int case_no) {

    cin >> n >> m;

    for (int i = 1; i <= m; ++i) {
        cin >> g[i].u >> g[i].v >> g[i].cost;
    }

    init_dsu();
    
    int current_components = n;
    long long ans = 0;
    
    while (current_components > 1) {
        
        for (int i = 1; i <= n; ++i) {
            min_edge[i] = -1;
        }
        
        for (int i = 1; i <= m; ++i) {

            int r_v = root(g[i].v);
            int r_u = root(g[i].u);

            if (r_v == r_u) continue;

            if (min_edge[r_u] == -1 || g[i].cost < g[min_edge[r_u]].cost) {
                min_edge[r_u] = i;
            }

            if (min_edge[r_v] == -1 || g[i].cost < g[min_edge[r_v]].cost) {
                min_edge[r_v] = i;
            }
        }

        for (int i = 1; i <= n; ++i) {

            if (min_edge[i] == -1) continue;

            if (merge(g[min_edge[i]].v, g[min_edge[i]].u)) {
                ans += g[min_edge[i]].cost;
                --current_components;
                // add g[min_edge[i]] to the final list of edges to be returned by this
            }

        }

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
