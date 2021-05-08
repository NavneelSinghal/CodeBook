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
    node_t combine(const node_t &n1, const node_t &n2) {
        return node_t{std::min(n1.mn, n2.mn), n1.sum + n2.sum, n1.sz + n2.sz};
    }

    // create node from base value and indices l, r
    node_t make_node(const base_t &val, int l, int r) { return {val, val, 1}; }

    // node corresponding to empty interval
    node_t id_node() { return {inf + 1, 0, 0}; }

    // apply update u to the whole node n
    node_t apply_update(const update_t &u, const node_t &nd) {
        // assume that updates are applied as assignments
        if (u == 0) return nd;  // id
        return {u, 1LL * u * nd.sz, nd.sz};
    }

    // effective update if v is applied to node, followed by u
    update_t compose_updates(const update_t &u, const update_t &v) {
        return {std::max(u, v)};
    }

    // identity update
    update_t id_update() { return 0; }

    std::vector<node_t> t;
    std::vector<update_t> lazy;
    int n;

    LazySegTree(std::vector<base_t> &a) {
        this->n = a.size();
        if (this->n == 0) return;
        this->t.assign(4 * a.size(), id_node());
        if constexpr (is_lazy) this->lazy.assign(2 * a.size(), id_update());
        _build(1, 0, n, a);
    }

    // half open
    void update(int l, int r, const update_t &u) {
        if constexpr (!is_lazy) assert(l == r - 1);
        _update(1, 0, n, l, r, u);
    }
    node_t query(int l, int r) { return _query(1, 0, n, l, r); }

    // find least R in [l, n] such that f(combine(a[l..r])) is false
    // and f(combine(a[l..r-1])) = true
    // Requires f to be contiguous (possibly empty) segments of true and false
    // b is true if stuff needs to be pushed, and false otherwise
    template <bool b = is_lazy, typename F>
    int first_false_right(int l, const F &f) {
        auto acc = id_node();
        assert(f(acc));
        auto i = _first_false_right<b, F>(1, 0, n, l, n, f, acc);
        if (i == -1) return n;
        return i;
    }

    // helper functions
    void _pullUp(int v) { t[v] = combine(t[2 * v], t[2 * v + 1]); }
    void _updateNode(int v, const update_t &u) {
        t[v] = apply_update(u, t[v]);
        if constexpr (is_lazy)
            if (v < (int)lazy.size()) lazy[v] = compose_updates(u, lazy[v]);
    }
    void _pushDown(int v) {
        if constexpr (!is_lazy) return;
        // for optimizing, try removing this maybe
        if (lazy[v] == id_update()) return;
        _updateNode(2 * v, lazy[v]);
        _updateNode(2 * v + 1, lazy[v]);
        lazy[v] = id_update();
    }

    // actual functions
    void _build(int v, int l, int r, const std::vector<base_t> &a) {
        if (l == r - 1) {
            t[v] = make_node(a[l], l, r);
            return;
        }
        int mid = (l + r) / 2;
        _build(2 * v, l, mid, a);
        _build(2 * v + 1, mid, r, a);
        _pullUp(v);
    }

    void _update(int v, int l, int r, int ql, int qr, const update_t &u) {
        if (qr <= l || r <= ql) return;  // empty intersection
        if (ql <= l && r <= qr) {        // completely inside query
            _updateNode(v, u);
            return;
        }
        _pushDown(v);
        int mid = (l + r) / 2;
        _update(2 * v, l, mid, ql, qr, u);
        _update(2 * v + 1, mid, r, ql, qr, u);
        _pullUp(v);
    }

    node_t _query(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) return id_node();  // empty intersection
        if (ql <= l && r <= qr) return t[v];       // completely inside query
        _pushDown(v);
        int mid = (l + r) / 2;
        return combine(_query(2 * v, l, mid, ql, qr),
                       _query(2 * v + 1, mid, r, ql, qr));
    }

    // find least R in [l, r] such that f(combine(a[ql..R])) is false
    // and f(combine(a[ql..R-1])) = true. -1 if not found
    // Requires f to be contiguous (possibly empty) segments of true and false
    // b = whether pushing is needed or not
    template <bool b = true, typename F>
    int _first_false_right(int v, int l, int r, int ql, int qr, const F &f,
                           node_t &acc) {
        if (r <= ql) return -1;
        if (qr <= l) return l;
        auto new_acc = combine(acc, t[v]);
        if (ql <= l && r <= qr) {
            if (f(new_acc)) {
                acc = new_acc;
                return -1;
            }
        }
        if (l == r - 1) return l;
        if constexpr (b) _pushDown(v);
        int mid = (r + l) / 2;
        auto res = _first_false_right<b, F>(2 * v, l, mid, ql, qr, f, acc);
        if (res != -1)
            return res;
        else
            return _first_false_right<b, F>(2 * v + 1, mid, r, ql, qr, f, acc);
    }
};

template <class node, node (*op)(node, node), node (*e)(), class fun,
          node (*eval)(fun, node), fun (*composition)(fun, fun), fun (*id)()>
class lazy_segtree {
   public:
    lazy_segtree(std::vector<node> v) : _n(v.size()), log_n(0) {
        while ((1 << log_n) < _n) ++log_n;
        size = (1 << log_n);
        data.assign(2 * size, e());
        lazy.assign(size, id());
        for (int i = 0; i < _n; ++i) data[size + i] = v[i];
        for (int i = size - 1; i >= 1; --i) update_one(i);
    }
    lazy_segtree(int t = 0, node x = e())
        : lazy_segtree(std::vector<node>(t, x)) {}
    node get(int pos) {
        int leaf = pos + size;
        push_anc(leaf);
        return data[leaf];
    }
    void set(int pos, node val) {
        int leaf = pos + size;
        push_anc(leaf);
        data[leaf] = val;
        update_anc(leaf);
    }
    node query_semi_open(int left, int right) {
        left += size, right += size;
        node res_left = e(), res_right = e();
        push_anc(left, left);
        push_anc(right, right);
        while (left < right) {
            if (left & 1) res_left = op(res_left, data[left++]);
            if (right & 1) res_right = op(data[--right], res_right);
            left >>= 1, right >>= 1;
        }
        return op(res_left, res_right);
    }
    node query_all() { return data[1]; }
    void apply_one(int pos, fun fct) {
        int leaf = pos + size;
        push_anc(leaf);
        data[leaf] = eval(fct, data[leaf]);
        update_anc(leaf);
    }
    void apply_semi_open(int left, int right, fun fct) {
        if (left == right) return;
        left += size, right += size;
        push_anc(left, left);
        push_anc(right - 1, right);
        int old_left = left, old_right = right;
        while (left < right) {
            if (left & 1) all_apply(left++, fct);
            if (right & 1) all_apply(--right, fct);
            left >>= 1, right >>= 1;
        }
        left = old_left, right = old_right;
        update_anc(left, left);
        update_anc(right - 1, right);
    }

   private:
    int _n, log_n, size;
    std::vector<node> data;
    std::vector<fun> lazy;
    void update_one(int k) { data[k] = op(data[k << 1], data[k << 1 | 1]); }
    void update_anc(int leaf, int dev = 1) {
        for (int i = 1 + __builtin_ctz(dev); i <= log_n; ++i)
            update_one(leaf >> i);
    }
    void all_apply(int k, fun fct) {
        data[k] = eval(fct, data[k]);
        if (k < size) lazy[k] = composition(fct, lazy[k]);
    }
    void push_one(int k) {
        all_apply(k << 1, lazy[k]);
        all_apply(k << 1 | 1, lazy[k]);
        lazy[k] = id();
    }
    void push_anc(int leaf, int dev = 1) {
        int s = 1 + __builtin_ctz(dev);
        for (int i = log_n; i >= s; --i) push_one(leaf >> i);
    }
};

namespace affine_sum {
    // data in the segment
    struct node_t {
        mint sum;
        int l, r;
    };
    // empty interval
    node_t e() { return {mint(0), 0, 0}; }
    // aggregate function to be applied for a query
    node_t combine(node_t a, node_t b) {
        if (a.l == 0) return b;
        if (b.l == 0) return a;
        a.r = b.r;
        a.sum += b.sum;
        return a;
    }
    // representation of a single update to be applied to all elements
    struct update_t {
        // b[i] -> u*b[i] + v*i + w
        mint u, v, w;
    };
    // how the segment changes after applying the update to all elements
    node_t update(update_t u, node_t n) {
        int l = n.l;
        int r = n.r;
        n.sum = u.u * n.sum + u.w * (r - l + 1) +
                u.v * (1LL * r * (r - 1) / 2) - u.v * (1LL * l * (l - 1) / 2);
        return n;
    }
    // how to compose the two updates - u1(u2(segment)), so first u2 then u1
    update_t compose_updates(update_t u1, update_t u2) {
        u1.w += u1.u * u2.w;
        u1.v += u1.u * u2.v;
        u1.u *= u2.u;
        return u1;
    }
    // return the identity update
    update_t id() { return {1, 0, 0}; }
    using LS =
        lazy_segtree<node_t, combine, e, update_t, update, compose_updates, id>;
}  // namespace affine_sum

/* structs end */

using namespace std;
using namespace __gnu_pbds;
// using namespace __gnu_cxx;

/* main code starts */

auto main() -> signed {
    setIO();
    int TESTS = 1;
    // cin >> TESTS;

    auto precompute = [&]() -> void {

    };

    auto solve = [&](int t) -> void {
        ignore_unused(t);
        int n, q;
        cin >> n >> q;
        vector<affine_sum::node_t> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i].sum.value;
            a[i].l = i + 1;
            a[i].r = i + 1;
        }
        affine_sum::LS seg(a);
        for (int i = 0; i < q; ++i) {
            int t;
            cin >> t;
            if (t == 0) {
                int l, r, b, c;
                cin >> l >> r >> b >> c;
                affine_sum::update_t upd = {b, 0, c};
                seg.apply_semi_open(l, r, upd);
            } else {
                int l, r;
                cin >> l >> r;
                cout << seg.query_semi_open(l, r).sum << '\n';
            }
        }
    };

    auto brute = [&]() -> void {

    };

    ignore_unused(brute);
    precompute();
    for (int _t = 1; _t <= TESTS; ++_t) {
        // cout << "Case #" << _t << ": ";
        solve(_t);
        // brute();
    }
    return 0;
}
