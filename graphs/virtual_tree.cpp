#pragma GCC optimize ("Ofast")
#pragma GCC target ("avx")
#pragma GCC optimize ("unroll-loops")

#include <bits/stdc++.h>

using namespace std;

#if 0

Problem statement: given Q queries of the form k x1 x2 ... xk, find \sum_{(i, j) \in {1..k}^2, i < j} dist(xi, xj)

Solution using virtual trees (compressed tree which contains only vertices and some LCAs (LCA is computed for adjacent vertices in preorder)

#endif

int n, l, timer;
vector<int> tin, tout, mark, dep, subtree_cnt;
vector<vector<int>> g, g_vt, up;

void dfs(int v, int p) {
    tin[v] = ++timer;
    up[v][0] = p;
    for (int i = 1; i <= l; ++i) up[v][i] = up[up[v][i - 1]][i - 1];
    for (int u : g[v]) {
        if (u != p) {
            dep[u] = dep[v] + 1;
            dfs(u, v);
        }
    }
    tout[v] = ++timer;
}

bool is_ancestor(int u, int v) {
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = l; i >= 0; --i) if (!is_ancestor(up[u][i], v)) u = up[u][i];
    return up[u][0];
}

void precompute_lca(int root = 0) {
    tin.resize(n);
    tout.resize(n);
    dep.resize(n);
    subtree_cnt.resize(n);
    timer = 0;
    l = ceil(log2(n));
    up.assign(n, vector<int>(l + 1));
    dfs(root, root);
}

bool cmp(int u, int v) { return tin[u] < tin[v]; }

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n;
    g.resize(n);
    g_vt.resize(n);
    mark.resize(n);

    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    precompute_lca();

    int q;
    cin >> q;
    while (q--) {
        
        int k;
        cin >> k;
        vector<int> vertices(k);
        
        for (auto &x : vertices) {
            cin >> x;
            --x;
            mark[x] = true;
        }
        
        // sort by dfs time (time when we enter vertex, also called tin-order), add lca of each pair of adjacent nodes, then sort again
        vector<int> v = vertices;
        sort(v.begin(), v.end(), cmp);
        for (int i = 1; i < k; ++i) v.push_back(lca(v[i - 1], v[i]));
        sort(v.begin(), v.end(), cmp);
        v.erase(unique(v.begin(), v.end()), v.end());

        for (auto x : v) {
            g_vt[x].clear();
        }

        // create virtual tree from the found order
        vector<int> st; // stack of vertices
        st.push_back(v[0]);
        for (int i = 1; i < (int)v.size(); ++i) {
            int u = v[i];
            while (st.size() >= 2 && !is_ancestor(st.back(), u)) {
                g_vt[st[st.size() - 2]].push_back(st.back());
                st.pop_back();
            }
            st.push_back(u);
        }

        while (st.size() >= 2) {
            g_vt[st[st.size() - 2]].push_back(st.back());
            st.pop_back();
        }

        int root_vt = st[0];

        function<long long(int, int)> solve = [&](int u, int p) {
            long long ans = 0;
            subtree_cnt[u] = mark[u];
            for (auto v : g_vt[u]) {
                ans += solve(v, u); // g_vt is directed tree so no parent check needed
                subtree_cnt[u] += subtree_cnt[v];
            }
            if (p != -1) {
                int weight = dep[u] - dep[p];
                ans += weight * 1ll * subtree_cnt[u] * 1ll * (k - subtree_cnt[u]);
            }
            return ans;
        };

        cout << solve(root_vt, -1) << '\n';

        for (auto &x : vertices) {
            mark[x] = false;
        }
    }

    return 0;
}
