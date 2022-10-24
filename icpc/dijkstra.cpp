template <class Key, class T>
struct radix_heap {
    static_assert(std::is_integral<Key>::value, "");
    std::array<std::vector<std::pair<Key, T>>, sizeof(Key) * 8 + 1> v;
    Key last = 0;
    size_t sz = 0, ptr = 0;
    static int lg(Key a) { return a ? std::__lg(a) : -1; }
    void emplace(Key key, T val) {
        assert(key >= last);
        v[lg(key ^ last) + 1].emplace_back(key, val);
        ++sz;
    }
    void pull() {
        if (ptr < v[0].size()) return;
        v[0].clear(), ptr = 0;
        int i = 1;
        while (v[i].empty()) ++i;
        last =
            std::min_element(std::begin(v[i]), std::end(v[i]),
                             [](auto a, auto b) { return a.first < b.first; })
                ->first;
        for (auto e : v[i]) v[lg(e.first ^ last) + 1].push_back(e);
        v[i].clear();
    }
    std::pair<Key, T> top() { return pull(), v[0][ptr]; }
    void pop() { pull(), --sz, ++ptr; }
    bool empty() const { return sz == 0; }
};

template <class T, class G>
auto dijkstra(const G& g, int s) {
    std::vector d(g.size(), std::numeric_limits<T>::max());
    std::vector prv(g.size(), -1);
    radix_heap<T, int> rh;
    rh.emplace(d[s] = 0, s);
    while (!rh.empty()) {
        auto [dv, v] = rh.top();
        rh.pop();
        if (dv != d[v]) continue;
        for (auto&& [to, w] : g[v]) {
            if (d[to] > dv + w) {
                d[to] = dv + w;
                rh.emplace(d[to], to);
                prv[to] = v;
            }
        }
    }
    return std::make_pair(d, prv);
}

namespace vanilla {
    template <typename T>
    using min_heap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

    template <class T, class G>
    auto dijkstra(const G& g, int s) {
        std::vector d(g.size(), std::numeric_limits<T>::max());
        std::vector prv(g.size(), -1);
        min_heap<std::pair<T, int>> rh;
        rh.emplace(d[s] = 0, s);
        while (!rh.empty()) {
            auto [dv, v] = rh.top();
            rh.pop();
            if (dv != d[v]) continue;
            for (auto&& [to, w] : g[v]) {
                if (d[to] > dv + w) {
                    d[to] = dv + w;
                    rh.emplace(d[to], to);
                    prv[to] = v;
                }
            }
        }
        return std::make_pair(d, prv);
    }
}  // namespace vanilla
