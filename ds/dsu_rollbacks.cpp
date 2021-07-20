struct DSURollbacks {
   private:
    // negative - size; happens at the root
    // non-negative - parent; happens at non-root
    std::vector<int> parent_or_size;
    std::vector<std::array<int, 4>> history;
    static constexpr int inf = 1e9;

   public:
    DSURollbacks(int n) : parent_or_size(n, -1) {}
    int root(int v) {
        if (parent_or_size[v] < 0) return v;
        return root(parent_or_size[v]);
    }
    bool join(int u, int v) {
        u = root(u);
        v = root(v);
        if (u == v) {
            history.emplace_back(std::array{inf, inf, inf, inf});
            return false;
        } else {
            if (-parent_or_size[u] < -parent_or_size[v]) std::swap(u, v);
            history.emplace_back(std::array{u, v, parent_or_size[u], parent_or_size[v]});
            parent_or_size[u] += parent_or_size[v];
            parent_or_size[v] = u;
            return true;
        }
    }
    void rollback() {
        auto [u, v, psu, psv] = history.back();
        history.pop_back();
        if (u != inf) parent_or_size[u] = psu, parent_or_size[v] = psv;
    }
};

