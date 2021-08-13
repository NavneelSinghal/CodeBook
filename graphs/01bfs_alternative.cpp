// Maintain two graphs - G0, G1 corresponding to the edges with weights 0 and 1
// respectively

vector<vector<int>> G0(n), G1(n);
vector<int> d(n), visited(n);
queue<int> q;

void dfs(int node) {
    // Traverse through the 0-connected component
    for (auto v : G0[node])
        if (!visited[v]) {
            visited[v] = 1;
            d[v] = d[node];
            dfs(v);
        }
    // Push the unvisited 1-cost neighbours
    for (auto v : G1[node])
        if (!visited[v]) {
            visited[v] = 1;
            d[v] = d[node] + 1;
            q.push(v);
        }
}

void bfs1_0(int start) {
    q.push(start);
    visited[start] = 1;

    while (!q.empty()) {
        int top = q.front();
        q.pop();
        dfs(top);
    }
}

