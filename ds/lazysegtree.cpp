template <bool is_lazy = true>
struct LazySegTree {
   
    struct node_t {
        int mn;
        long long sum;
        int sz;
    };

    using base_t = int;
    using update_t = int;

    // combining two nodes
    inline node_t combine(const node_t &n1, const node_t &n2) const {
        return node_t{std::min(n1.mn, n2.mn), n1.sum + n2.sum, n1.sz + n2.sz};
    }

    // create node from base value and index i
    inline node_t make_node(const base_t &val, int i) const {
        return {val, val, 1};
    }

    // node corresponding to empty interval
    inline node_t id_node() const { return {inf + 1, 0, 0}; }

    // apply update u to the whole node n
    inline node_t apply_update(const update_t &u, const node_t &nd) const {
        // assume that updates are applied as assignments
        if (u == 0) return nd;  // id
        return {u, 1LL * u * nd.sz, nd.sz};
    }

    // effective update if v is applied to node, followed by u
    inline update_t compose_updates(const update_t &u,
                                    const update_t &v) const {
        return {std::max(u, v)};
    }

    // identity update
    inline update_t id_update() const { return 0; }

    std::vector<node_t> t;
    std::vector<update_t> lazy;
    int n;

    LazySegTree(std::vector<base_t> &a) {
        this->n = (int)a.size();
        if (this->n == 0) return;
        this->t.assign(2 * a.size() - 1, id_node());
        if constexpr (is_lazy) this->lazy.assign(2 * a.size() - 1, id_update());
        _build(0, 0, n, a);
    }

    // half open
    void update(int l, int r, const update_t &u) {
        if constexpr (!is_lazy) assert(l == r - 1);
        ql = l, qr = r;
        if (l >= r) return;
        _update(0, 0, n, u);
    }
    node_t query(int l, int r) {
        ql = l, qr = r;
        if (l >= r) return id_node();
        return _query(0, 0, n);
    }

    // find least R in [l, n] such that f(combine(a[l..r])) is false
    // and f(combine(a[l..r-1])) = true
    // f = [true, true, ...., false, false] (number of true, false can be 0)
    // b is true if stuff needs to be pushed, and false otherwise
    template <bool b = is_lazy, typename F>
    int first_false_right(int l, const F &f) {
        auto acc = id_node();
        // assert(f(acc));
        ql = l, qr = n;
        auto i = _first_false_right<b, F>(0, 0, n, f, acc);
        if (i == -1) return n;
        return i;
    }

   private:
    int ql, qr;
    // helper functions
    inline void _pullUp(int v, int l, int m) {
        t[v] = combine(t[v + 1], t[v + ((m - l) << 1)]);
    }
    inline void _updateNode(int v, const update_t &u) {
        t[v] = apply_update(u, t[v]);
        if constexpr (is_lazy) lazy[v] = compose_updates(u, lazy[v]);
    }
    inline void _pushDown(int v, int l, int m) {
        if constexpr (!is_lazy) return;
        if (lazy[v] == id_update()) return;
        _updateNode(v + 1, lazy[v]);
        _updateNode(v + ((m - l) << 1), lazy[v]);
        lazy[v] = id_update();
    }

    // actual functions
    void _build(int v, int l, int r, const std::vector<base_t> &a) {
        if (l == r - 1) {
            t[v] = make_node(a[l], l);
            return;
        }
        int m = (l + r) / 2;
        _build(v + 1, l, m, a);
        _build(v + ((m - l) << 1), m, r, a);
        _pullUp(v, l, m);
    }

    // only go down branches with at non-empty intersection, same for _query
    void _update(int v, int l, int r, const update_t &u) {
        if (ql <= l && r <= qr) {  // completely inside query
            _updateNode(v, u);
            return;
        }
        int m = (l + r) / 2;
        _pushDown(v, l, m);
        if (ql < m) _update(v + 1, l, m, u);
        if (m < qr) _update(v + ((m - l) << 1), m, r, u);
        _pullUp(v, l, m);
    }

    node_t _query(int v, int l, int r) {
        if (ql <= l && r <= qr) return t[v];  // completely inside query
        int m = (l + r) / 2;
        _pushDown(v, l, m);
        if (m >= qr) return _query(v + 1, l, m);
        if (m <= ql) return _query(v + ((m - l) << 1), m, r);
        return combine(_query(v + 1, l, m), _query(v + ((m - l) << 1), m, r));
    }

    // find least R in [l, r] such that f(combine(a[ql..R])) is false
    // and f(combine(a[ql..R-1])) = true. -1 if not found
    template <bool b = true, typename F>
    int _first_false_right(int v, int l, int r, const F &f, node_t &acc) {
        if (r <= ql) return -1;
        if (qr <= l) return l;
        auto new_acc = combine(acc, t[v]);
        if (ql <= l && r <= qr && f(new_acc)) {
            acc = new_acc;
            return -1;
        }
        if (l == r - 1) return l;
        int m = (r + l) / 2;
        if constexpr (b) _pushDown(v, l, m);
        // auto res = _first_false_right<b, F>(v + 1, l, m, f, acc);
        // if (res != -1)
        //     return res;
        // else
        //     return _first_false_right<b, F>(v + ((m - l) << 1), m, r, f,
        //     acc);
        if (ql < m) {
            auto res = _first_false_right<b, F>(v + 1, l, m, f, acc);
            if (res != -1) return res;
        }
        if (m < qr)
            return _first_false_right<b, F>(v + ((m - l) << 1), m, r, f, acc);
        return -1;
    }
};

