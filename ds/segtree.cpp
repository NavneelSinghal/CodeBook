template <class S, S (*op)(S, S), S (*e)()>
struct segtree {
   public:
    segtree() : segtree(0) {}
    segtree(int n) : segtree(std::vector<S>(n, e())) {}
    segtree(const std::vector<S>& v) : _n(int(v.size())) {
        log = 0;
        size = 1;
        while (size < _n) {
            size <<= 1;
            log++;
        }
        d = std::vector<S>(2 * size, e());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) {
        assert(0 <= p && p < _n);
        return d[p + size];
    }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= _n);
        S sml = e(), smr = e();
        l += size;
        r += size;

        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return op(sml, smr);
    }

    S all_prod() { return d[1]; }

    template <bool (*f)(S)>
    int max_right(int l) {
        return max_right(l, [](S x) { return f(x); });
    }
    template <class F>
    int max_right(int l, F f) {
        assert(0 <= l && l <= _n);
        assert(f(e()));
        if (l == _n) return _n;
        l += size;
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(sm, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <bool (*f)(S)>
    int min_left(int r) {
        return min_left(r, [](S x) { return f(x); });
    }
    template <class F>
    int min_left(int r, F f) {
        assert(0 <= r && r <= _n);
        assert(f(e()));
        if (r == 0) return 0;
        r += size;
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

   private:
    int _n, size, log;
    std::vector<S> d;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
};

int combine(int a, int b) { return min(a, b); }

int id() { return 1e9; }

using SegTree = segtree<int, combine, id>;

// some issue with iterative segtree - see https://codeforces.com/contest/1420
// C2

// implementation with optimal space
// to allow for O(1) query for [0..n) by t[1], see the segtree below this one

// TODO: implement binary search on SegTree, and space optimized version

template <typename node = long long, typename T = long long>
struct SegTree {

    // change this
    node combine(node n1, node n2) { return n1 + n2; }
    node make_node(T val) { return val; }
    
    const node ID = {};
    vector<node> t;
    int n;

    SegTree(vector<T>& a) {
        this->n = a.size();
        this->t.resize(4 * a.size() + 4);
        _build(1, 0, n - 1, a);
    }

    void update(int i, T val) { _update(1, 0, n - 1, i, val); }
    node query(int l, int r) { return _query(1, 0, n - 1, l, r); }

    void _build(int v, int l, int r, vector<T>& a) {
        if (l == r) {
            t[v] = make_node(a[l]);
            return;
        }
        int mid = (l + r) >> 1;
        _build(v << 1, l, mid, a);
        _build((v << 1) | 1, mid + 1, r, a);
        t[v] = combine(t[(v << 1)], t[(v << 1) | 1]);
    }

    void _update(int v, int l, int r, int idx, T val) {
        if (l == r) {
            t[v] = make_node(val);
            return;
        }
        int mid = (l + r) >> 1;
        if (idx <= mid)
            _update(v << 1, l, mid, idx, val);
        else
            _update((v << 1) | 1, mid + 1, r, idx, val);
        t[v] = combine(t[v << 1], t[(v << 1) | 1]);
    }

    node _query(int v, int tl, int tr, int l, int r) {
        if (l == tl && r == tr) return t[v];
        int tm = (tl + tr) >> 1;
        if (l > tm) return _query((v << 1) | 1, tm + 1, tr, l, r);
        if (tm + 1 > r) return _query(v << 1, tl, tm, l, r);
        return combine(_query(v << 1, tl, tm, l, tm),
                       _query((v << 1) | 1, tm + 1, tr, tm + 1, r));
    }
};

struct SegTree {
    typedef int T;
    typedef int S;
    static constexpr T unit = 0;
    T make_node(S val) { return val; }
    T combine(T a, T b) { return a + b; }
    void update_val(T& a, S b) { a += b; }

    vector<T> t;
    int32_t n;

    SegTree(int32_t n = 0, T def = unit) : t(n << 1, def), n(n) {}
    SegTree(vector<S>& a, T def = unit) {
        n = a.size();
        t.assign(n << 1, unit);
        for (int32_t i = 0; i < n; ++i) {
            t[i + n] = make_node(a[i]);
        }
        for (int32_t i = n - 1; i; --i) {
            t[i] = combine(t[i << 1], t[i << 1 | 1]);
        }
    }

    void update(int32_t pos, S val) {
        for (update_val(t[pos += n], val); pos >>= 1;) {
            t[pos] = combine(t[pos << 1], t[pos << 1 | 1]);
        }
    }

    T query(int32_t l, int32_t r) {
        T ra = unit, rb = unit;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ra = combine(ra, t[l++]);
            if (r & 1) rb = combine(t[--r], rb);
        }
        return combine(ra, rb);
    }
};

struct SegTree {
    // datatype of nodes of segment tree
    typedef int T;
    // datatype of vector that's generating the segment tree
    typedef int S;
    // identity element of monoid
    // if you have any issues with unit, define it outside the struct as a
    // normal variable
    static constexpr T unit = 0;
    // node of segment tree from a value
    T make_node(S val) { return val; }
    // combine function - needs to be an associative function
    T combine(T a, T b) { return a + b; }
    // point update function - updating the element in the array
    void update_val(T& a, S b) { a += b; }

    vector<T> t;
    int32_t n;
    int32_t N;

    SegTree(int32_t n = 0, T def = unit) : n(n) {
        N = 1;
        while (N < n) N <<= 1;
        t.assign(N << 1, def);
    }

    SegTree(vector<S>& a, T def = unit) {
        n = a.size();
        N = 1;
        while (N < n) N <<= 1;
        t.assign(N << 2, unit);
        for (int32_t i = 0; i < n; ++i) {
            t[i + N] = make_node(a[i]);
        }
        for (int32_t i = n - 1; i; --i) {
            t[i] = combine(t[i << 1], t[i << 1 | 1]);
        }
    }

    void update(int32_t pos, S val) {
        for (update_val(t[pos += N], val); pos >>= 1;) {
            t[pos] = combine(t[pos << 1], t[pos << 1 | 1]);
        }
    }

    T query(int32_t l, int32_t r) {
        T ra = unit, rb = unit;
        for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ra = combine(ra, t[l++]);
            if (r & 1) rb = combine(t[--r], rb);
        }
        return combine(ra, rb);
    }
};


