// finds euler tour, euler path in directed and undirected graphs
// assumes there exists such a path
// conditions:
// undirected:
//   tour: all degrees are even
//   path: all degrees are even except the degrees of endpoints
// directed:
//   tour: indegree = outdegree for everything
//   path: if starting, indeg = outdeg - 1, if ending, indeg = outdeg + 1, and
//         indeg = outdeg otherwise
template <bool is_directed = false>
class EulerTour {
   private:
    vector<vector<int>> g;
    vector<int> edge_ptr;

    void dfs_tour(int i, vector<int>& que) {
        while (edge_ptr[i] < (int)g[i].size()) {
            int t = g[i][edge_ptr[i]];
            ++edge_ptr[i];
            if ((!is_directed) &&
                (edge_ptr[t] >= (int)g[t].size() || g[t][edge_ptr[t]] > i))
                continue;
            dfs_tour(t, que);
        }
        que.push_back(i);
    }

   public:
    // dir = true if graph is directed
    EulerTour(const vector<vector<int>>& g_) : g(g_), edge_ptr(g_.size(), 0) {
        for (int i = 0; i < (int)g.size(); ++i) sort(g[i].begin(), g[i].end());
    }

    vector<int> get_tour(int i = 0) {
        vector<int> res;
        dfs_tour(i, res);
        reverse(begin(res), end(res));
        return res;
    }
};

