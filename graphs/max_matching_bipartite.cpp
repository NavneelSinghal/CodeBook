// consider shuffling adjacency lists if this gives TLE
template <bool ToShuffle = false>
struct bipartite_matching {
    int n_left, n_right;
    std::vector<std::vector<int>> g;
    std::vector<int> match_from_left, match_from_right;
    std::vector<int> vis;
    int iteration;

    bipartite_matching(int _n_left, int _n_right)
        : n_left(_n_left),
          n_right(_n_right),
          g(_n_left),
          vis(_n_left, 0),
          match_from_left(_n_left, -1),
          match_from_right(_n_right, -1),
          iteration(0) {}

    // u on left, v on right
    void add(int u, int v) { g[u].push_back(v); }

    bool dfs(int u) {
        vis[u] = iteration;
        for (auto v : g[u]) {
            if (match_from_right[v] == -1) {
                match_from_right[v] = u;
                match_from_left[u] = v;
                return true;
            }
        }
        for (auto v : g[u]) {
            if (vis[match_from_right[v]] != iteration &&
                dfs(match_from_right[v])) {
                match_from_right[v] = u;
                match_from_left[u] = v;
                return true;
            }
        }
        return false;
    }

    int get_max_matching() {
        if constexpr (ToShuffle) {
            Random rng;
            for (int i = 0; i < n_left; ++i)
                std::random_shuffle(std::begin(g[i]), std::end(g[i]), rng);
        }
        int new_matchings = 0;
        int matchings = 0;
        do {
            iteration++;
            new_matchings = 0;
            for (int u = 0; u < n_left; ++u)
                if (match_from_left[u] == -1 && dfs(u)) new_matchings++;
            matchings += new_matchings;
        } while (new_matchings > 0);
        return matchings;
    }
};

struct Random : std::mt19937 {
    using std::mt19937::mt19937;
    using std::mt19937::operator();
    static int64_t gen_seed() {
        return std::chrono::steady_clock::now().time_since_epoch().count();
    }
    Random() : std::mt19937(gen_seed()) {}
    template <class Int>
    auto operator()(Int a, Int b)
        -> std::enable_if_t<std::is_integral_v<Int>, Int> {
        return std::uniform_int_distribution<Int>(a, b)(*this);
    }
    template <class Int>
    auto operator()(Int a) -> std::enable_if_t<std::is_integral_v<Int>, Int> {
        return std::uniform_int_distribution<Int>(0, a - 1)(*this);
    }
    template <class Real>
    auto operator()(Real a, Real b)
        -> std::enable_if_t<std::is_floating_point_v<Real>, Real> {
        return std::uniform_real_distribution<Real>(a, b)(*this);
    }
};

//// faster if the previous one fails
// template <bool ToShuffle = false>
// struct bipartite_matching {
//     int n_left, n_right, flow = 0;
//     std::vector<std::vector<int>> g;
//     std::vector<int> match_from_left, match_from_right;
// 
//     bipartite_matching(int _n_left, int _n_right)
//         : n_left(_n_left),
//           n_right(_n_right),
//           g(_n_left),
//           match_from_left(_n_left, -1),
//           match_from_right(_n_right, -1),
//           dist(_n_left) {}
// 
//     void add(int u, int v) { g[u].push_back(v); }
// 
//     std::vector<int> dist;
// 
//     void bfs() {
//         std::queue<int> q;
//         for (int u = 0; u < n_left; ++u) {
//             if (!~match_from_left[u])
//                 q.push(u), dist[u] = 0;
//             else
//                 dist[u] = -1;
//         }
//         while (!q.empty()) {
//             int u = q.front();
//             q.pop();
//             for (auto v : g[u])
//                 if (~match_from_right[v] && !~dist[match_from_right[v]]) {
//                     dist[match_from_right[v]] = dist[u] + 1;
//                     q.push(match_from_right[v]);
//                 }
//         }
//     }
// 
//     bool dfs(int u) {
//         for (auto v : g[u])
//             if (!~match_from_right[v]) {
//                 match_from_left[u] = v, match_from_right[v] = u;
//                 return true;
//             }
//         for (auto v : g[u])
//             if (dist[match_from_right[v]] == dist[u] + 1 &&
//                 dfs(match_from_right[v])) {
//                 match_from_left[u] = v, match_from_right[v] = u;
//                 return true;
//             }
//         return false;
//     }
// 
//     int get_max_matching() {
//         if constexpr (ToShuffle) {
//             Random rng;
//             for (int i = 0; i < n_left; ++i)
//                 std::random_shuffle(std::begin(g[i]), std::end(g[i]), rng);
//         }
//         while (true) {
//             bfs();
//             int augment = 0;
//             for (int u = 0; u < n_left; ++u)
//                 if (!~match_from_left[u]) augment += dfs(u);
//             if (!augment) break;
//             flow += augment;
//         }
//         return flow;
//     }
// 
//     std::pair<std::vector<int>, std::vector<int>> minimum_vertex_cover() {
//         std::vector<int> L, R;
//         for (int u = 0; u < n_left; ++u) {
//             if (!~dist[u])
//                 L.push_back(u);
//             else if (~match_from_left[u])
//                 R.push_back(match_from_left[u]);
//         }
//         return {L, R};
//     }
// 
//     std::vector<std::pair<int, int>> get_edges() {
//         std::vector<std::pair<int, int>> ans;
//         for (int u = 0; u < n_left; ++u)
//             if (match_from_left[u] != -1)
//                 ans.emplace_back(u, match_from_left[u]);
//         return ans;
//     }
// };

