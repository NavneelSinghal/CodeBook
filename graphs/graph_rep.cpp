struct graph_edge_pointers {
    
    struct edge {
        int to, nxt, disable; // to = other vertex, nxt = index of next edge from cur vertex
        edge(int to, int nxt) : to(to), nxt(nxt), disable(0) {}
    };

    vector<int> head; // head[i] = index of first edge emanating from vertex i
    vector<edge> edges;
    vector<int> siz;

    graph_edge_pointers(int n, int m) : head(n, -1), siz(n) { edges.reserve(m); }

    // while adding (u, v), (v, u), we have i, i^1 as corresponding edges
    void add_edge(int u, int v) {
        edges.emplace_back(v, head[u]);
        head[u] = (int)edges.size() - 1;
    }

    // example code
    // function<bool(int, int, int)> dfs = [&] (int v, int p, int f) {
    //     if (f <= 3) return true;
    //     g.siz[v] = 1;
    //     for (int i = g.head[v]; ~i; i = g.edges[i].nxt) {
    //         if (g.edges[i].to != p && !g.edges[i].disable) {
    //             int to = g.edges[i].to;
    //             if (dfs(to, v, f))
    //                 return true;
    //             int siz_child = g.siz[to];
    //             if (siz_child == fib[f - 1]) {
    //                 g.edges[i].disable = 1;
    //                 g.edges[i ^ 1].disable = 1;
    //                 return dfs(to, to, f - 1) && dfs(v, v, f - 2);
    //             } else if (siz_child == fib[f - 2]) {
    //                 g.edges[i].disable = 1;
    //                 g.edges[i ^ 1].disable = 1;
    //                 return dfs(to, to, f - 2) && dfs(v, v, f - 1);
    //             }
    //             g.siz[v] += siz_child;
    //         }
    //     }
    //     return false;
    // };

};


