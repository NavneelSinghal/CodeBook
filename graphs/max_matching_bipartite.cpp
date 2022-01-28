struct Random : std::mt19937 {
    using std::mt19937::mt19937;
    using std::mt19937::operator();
    static std::int64_t gen_seed() {
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

template <bool ToShuffle = false>
struct bipartite_matching {
   public:
    bipartite_matching(int _n_left, int _n_right)
        : n_left(_n_left),
          n_right(_n_right),
          g(_n_left),
          match_from_left(_n_left, -1),
          match_from_right(_n_right, -1),
          vis(_n_left) {}

    // u on left, v on right
    void add(int u, int v) { g[u].push_back(v); }
    void reset() {
        matchings = 0, computed = false, iteration = 0;
        std::fill(vis.begin(), vis.end(), 0);
    }
    int get_max_matching() {
        if (!computed) compute_max_matching();
        return matchings;
    }

    std::vector<std::pair<int, int>> get_edges() {
        if (!computed) compute_max_matching();
        std::vector<std::pair<int, int>> ans;
        for (int u = 0; u < n_left; ++u)
            if (~match_from_left[u]) ans.emplace_back(u, match_from_left[u]);
        return ans;
    }

    std::pair<std::vector<int>, std::vector<int>> minimum_vertex_cover() {
        if (!computed) compute_max_matching();
        std::vector<int> L, R;
        simple_queue<int> q;
        for (int u = 0; u < n_left; ++u)
            if (!~match_from_left[u]) q.push(u), vis[u] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto v : g[u]) {
                int U = match_from_right[v];
                if (~U && !vis[U]) {
                    vis[U] = 1;
                    q.push(U);
                }
            }
        }
        for (int u = 0; u < n_left; ++u) {
            if (!vis[u])
                L.push_back(u);
            else if (~match_from_left[u])
                R.push_back(match_from_left[u]);
        }
        return {L, R};
    }

   private:
    template <class T>
    struct simple_queue {
        std::vector<T> payload;
        int pos = 0;
        void reserve(int n) { payload.reserve(n); }
        int size() const { return int(payload.size()) - pos; }
        bool empty() const { return pos == int(payload.size()); }
        void push(const T& t) { payload.push_back(t); }
        T& front() { return payload[pos]; }
        void clear() { payload.clear(), pos = 0; }
        void pop() { pos++; }
    };

    int n_left, n_right;
    std::vector<std::basic_string<int>> g;
    std::vector<int> match_from_left, match_from_right;
    std::vector<int> vis;
    int iteration = 0;
    int matchings = 0;
    bool computed = false;

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

    void compute_max_matching() {
        if constexpr (ToShuffle) {
            Random rng;
            for (int i = 0; i < n_left; ++i)
                std::random_shuffle(std::begin(g[i]), std::end(g[i]), rng);
        }
        while (true) {
            int new_matchings = 0;
            iteration++;
            for (int u = 0; u < n_left; ++u)
                if (!~match_from_left[u] && dfs(u)) new_matchings++;
            if (!new_matchings) break;
            matchings += new_matchings;
        }
        computed = true;
    }
};

template <bool ToShuffle = false>
struct bipartite_matching_hopcroft_karp {
   public:
    bipartite_matching_hopcroft_karp(int _n_left, int _n_right)
        : n_left(_n_left),
          n_right(_n_right),
          g(_n_left),
          match_from_left(_n_left, -1),
          match_from_right(_n_right, -1),
          dist(_n_left) {}

    void add(int u, int v) { g[u].push_back(v); }

    void reset() { matchings = 0, computed = false; }

    int get_max_matching() {
        if (!computed) compute_max_matching();
        return matchings;
    }

    std::pair<std::vector<int>, std::vector<int>> minimum_vertex_cover() {
        if (!computed) compute_max_matching();
        std::vector<int> L, R;
        for (int u = 0; u < n_left; ++u) {
            if (!~dist[u])
                L.push_back(u);
            else if (~match_from_left[u])
                R.push_back(match_from_left[u]);
        }
        return {L, R};
    }

    std::vector<std::pair<int, int>> get_edges() {
        if (!computed) compute_max_matching();
        std::vector<std::pair<int, int>> ans;
        for (int u = 0; u < n_left; ++u)
            if (~match_from_left[u]) ans.emplace_back(u, match_from_left[u]);
        return ans;
    }

   private:
    template <class T>
    struct simple_queue {
        std::vector<T> payload;
        int pos = 0;
        void reserve(int n) { payload.reserve(n); }
        int size() const { return int(payload.size()) - pos; }
        bool empty() const { return pos == int(payload.size()); }
        void push(const T& t) { payload.push_back(t); }
        T& front() { return payload[pos]; }
        void clear() { payload.clear(), pos = 0; }
        void pop() { pos++; }
    };

    int n_left, n_right, matchings = 0;
    bool computed = false;
    std::vector<std::basic_string<int>> g;
    std::vector<int> match_from_left, match_from_right;
    std::vector<int> dist;

    void bfs() {
        simple_queue<int> q;
        for (int u = 0; u < n_left; ++u) {
            if (!~match_from_left[u])
                q.push(u), dist[u] = 0;
            else
                dist[u] = -1;
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto v : g[u]) {
                int U = match_from_right[v];
                if (~U && !~dist[U]) {
                    dist[U] = dist[u] + 1;
                    q.push(U);
                }
            }
        }
    }

    bool dfs(int u) {
        for (auto v : g[u])
            if (!~match_from_right[v]) {
                match_from_left[u] = v, match_from_right[v] = u;
                return true;
            }
        for (auto v : g[u])
            if (dist[match_from_right[v]] == dist[u] + 1 &&
                dfs(match_from_right[v])) {
                match_from_left[u] = v, match_from_right[v] = u;
                return true;
            }
        return false;
    }

    void compute_max_matching() {
        if constexpr (ToShuffle) {
            Random rng;
            for (int i = 0; i < n_left; ++i)
                std::random_shuffle(std::begin(g[i]), std::end(g[i]), rng);
        }
        while (true) {
            bfs();
            int new_matchings = 0;
            for (int u = 0; u < n_left; ++u)
                if (!~match_from_left[u] && dfs(u)) ++new_matchings;
            if (!new_matchings) break;
            matchings += new_matchings;
        }
        computed = true;
    }
};

