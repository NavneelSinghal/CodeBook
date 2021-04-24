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
