int n;                    // number of nodes
vector<vector<int>> adj;  // adjacency list of graph

// vector<bool> visited;
vector<int> tin, low;
int timer;

// low[v] is the minimum tin for all vertices reachable from the subtree of the
// dfs tree rooted at v

void dfs(int v, int p = -1) {
    // visited[v] = true;
    tin[v] = low[v] = ++timer;

    for (int to : adj[v]) {
        if (to == p) continue;
        if (tin[to]) {
            low[v] = min(low[v], tin[to]);
        } else {
            dfs(to, v);
            low[v] = min(low[v], low[to]);
            if (low[to] > tin[v]) IS_BRIDGE(v, to);
        }
    }
}

void find_bridges() {
    timer = 0;
    // visited.assign(n, false);
    tin.assign(n, 0);
    low.assign(n, 0);
    for (int i = 0; i < n; ++i) {
        if (!tin[i]) dfs(i);
    }
}
