#pragma GCC optimize ("Ofast")
#include <bits/stdc++.h>

using namespace std;

// https://codeforces.com/gym/100712/attachments/download/3454/acm-amman-collegiate-programming-contest-en.pdf
// problem H

/*

bridge tree -
1. each vertex is in at most 1 bridge component - proof: suppose not, then contradict maximality
2. each vertex is in at least 1 bridge component - proof: set of relaxed bridge components containing a vertex is non-empty, so at least 1 maximal bridge component
3. compressing bridge components into vertex makes it a forest (or tree) - proof: suppose cycle. then show contradiction.

construction -
perform a dfs on the bridge tree - by running bfs inside a dfs, and calling dfs with a new component only when you encounter a bridge edge

*/

int main() {
    
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int t;
    cin >> t;

    while (t--) {
        
        int n, m;
        cin >> n >> m;
        
        // graph
        vector<vector<int>> g(n); // g[i] contains indices of edges
        vector<int> U(m), V(m); // U[i], V[i] = i^th edge

        // adjacent(u, i) when called with u = one member of the i^th edge returns the other member
        auto adjacent = [&] (int u, int i) {
            return (u != U[i]) ? U[i] : V[i];
        };

        // bridge detection - fails for multiple edges if index in dfs_bridge is replaced by parent
        vector<char> is_bridge(m);
        vector<int> tin(n), low(n);
        int timer = 0;

        for (int i = 0; i < m; ++i) {
            int u, v;
            cin >> u >> v;
            --u, --v;
            // store edge index
            g[u].push_back(i);
            g[v].push_back(i);
            U[i] = u;
            V[i] = v;
        }

        function<void(int, int)> dfs_bridge = [&] (int u, int index) {

            tin[u] = low[u] = ++timer;

            for (auto i : g[u]) {
                int v = adjacent(u, i);
                if (index == i) continue;
                if (tin[v]) {
                    low[u] = min(low[u], tin[v]);
                } else {
                    dfs_bridge(v, i);
                    low[u] = min(low[u], low[v]);
                    if (low[v] > tin[u]) {
                        is_bridge[i] = true;
                    }
                }
            }
            
        };

        dfs_bridge(0, -1); // sentinel index

        // now all the edges which are bridges are marked

        int global_component = 0;
        vector<char> visited(n);
        vector<vector<int>> tree(1);

        // vector<int> component_of(n);

        function<void(int)> dfs_build_bridge_tree = [&] (int u) {
            
            int current_component = global_component;
            
            queue<int> q;
            q.push(u);
            
            visited[u] = 1;
            
            while (!q.empty()) {
                
                int v = q.front();
                q.pop();
                
                // component_of[v] = current_component;
                
                for (auto i : g[v]) {
                    int w = adjacent(v, i);
                    if (!visited[w]) {
                        if (is_bridge[i]) {
                            global_component++;
                            tree.push_back(vector<int>());
                            tree[global_component].push_back(current_component);
                            tree[current_component].push_back(global_component);
                            dfs_build_bridge_tree(w);
                        } else {
                            visited[w] = 1;
                            q.push(w);
                        }
                    }
                }
            }
        };

        dfs_build_bridge_tree(0);

        // now find the diameter of the tree
        // for this run a bfs from 0 to all vertices and find the farthest vertex
        // then run a bfs from the farthest vertex to all the vertices and find the maximum distance
        
        const int INF = 1e9;

        vector<int> dist(tree.size(), INF);
        dist[0] = 0;
        
        queue<int> q;
        q.push(0);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto v : tree[u]) {
                if (dist[v] == INF) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }

        int index = max_element(dist.begin(), dist.end()) - dist.begin();

        // for (auto &x : dist) x = INF;
        fill(dist.begin(), dist.end(), INF);
        dist[index] = 0;
        
        // queue<int>().swap(q);
        q.push(index);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto v : tree[u]) {
                if (dist[v] == INF) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }

        int max_distance = *max_element(dist.begin(), dist.end());
        cout << (tree.size() - max_distance - 1) << endl;

    }

}
