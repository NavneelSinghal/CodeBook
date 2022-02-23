// https://github.com/Aeren1564/Algorithms/blob/master/Algorithm_Implementations_Cpp/Graph_Theory/DFS_Based/two_sat_solver.sublime-snippet
struct two_sat_solver {
    int n;
    vector<vector<int>> adj;
    vector<int> value, val, comp, z;
    two_sat_solver(int n = 0) : n(n), adj(n << 1) {}
    int time = 0;
    int add_variable() {
        adj.emplace_back();
        adj.emplace_back();
        return n++;
    }
    void either(int u, int v) {
        u = max(2 * u, -1 - 2 * u);
        v = max(2 * v, -1 - 2 * v);
        adj[u].push_back(v ^ 1);
        adj[v].push_back(u ^ 1);
    }
    void implies(int u, int v) { either(~u, v); }
    void one_of(int u, int v) {
        either(u, v);
        either(~u, ~v);
    }
    void set_value(int u) { either(u, u); }
    void at_most_one(const vector<int>& arr) {
        if ((int)arr.size() <= 1) return;
        int cur = ~arr[0];
        for (auto u = 2; u < (int)arr.size(); ++u) {
            int next = add_variable();
            either(cur, ~arr[u]), either(cur, next), either(~arr[u], next);
            cur = ~next;
        }
        either(cur, ~arr[1]);
    }
    int dfs(int u) {
        int low = val[u] = ++time, v;
        z.push_back(u);
        for (auto v : adj[u])
            if (!comp[v]) low = min(low, val[v] ?: dfs(v));
        ++time;
        if (low == val[u]) do {
                v = z.back();
                z.pop_back();
                comp[v] = time;
                if (value[v >> 1] == -1) value[v >> 1] = v & 1;
            } while (v != u);
        return val[u] = low;
    }
    // O(n)
    bool solve() {
        value.assign(n, -1), val.assign(2 * n, 0), comp = val;
        for (auto u = 0; u < n << 1; ++u)
            if (!comp[u]) dfs(u);
        for (auto u = 0; u < n; ++u)
            if (comp[u << 1] == comp[u << 1 ^ 1]) return false;
        return true;
    }
    bool get_value(int u) { return value[u]; }
};

