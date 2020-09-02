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

typedef struct {
    // datatype
} node;

const node ID;
node t[4 * maxn];

node combine(node n1, node n2) {
    node ans;
    // do something
    return ans;
}

node make_node(int val) {
    // make a node from val and return
}

// build segtree - 1 indexed
void build(int v, int l, int r, vector<int>& a) {
    if (l == r) {
        t[v] = make_node(a[l]);
        return;
    }
    int mid = (l + r) >> 1;
    build(v << 1, l, mid, a);
    build((v << 1) | 1, mid + 1, r, a);
    t[v] = combine(t[(v << 1)], t[(v << 1) | 1]);
}

// update segtree by updating value to val at idx
void update(int v, int l, int r, int idx, int val) {
    if (l == r) {
        t[v] = make_node(val);
        return;
    }
    int mid = (l + r) >> 1;
    if (idx <= mid)
        update(v << 1, l, mid, idx, val);
    else
        update((v << 1) | 1, mid + 1, r, idx, val);
    t[v] = combine(t[v << 1], t[(v << 1) | 1]);
}

// range query from l to r both inclusive
node query(int v, int tl, int tr, int l, int r) {
    if (l > r) return ID;
    if (l == tl && r == tr) {
        return t[v];
    }
    int tm = (tl + tr) >> 1;
    return combine(query(v << 1, tl, tm, l, min(r, tm)),
            query((v << 1) | 1, tm + 1, tr, max(l, tm + 1), r));
}

// slightly more efficient range query
node query2(int v, int tl, int tr, int l, int r) {
    if (l == tl && r == tr)
        return t[v];
    int tm = (tl + tr) >> 1;
    if (l > tm)
        return query((v << 1) | 1, tm + 1, tr, l, r);
    if (tm + 1 > r)
        return query(v << 1, tl, tm, l, r);
    return combine(query(v << 1, tl, tm, l, tm),
            query((v << 1) | 1, tm + 1, tr, tm + 1, r));
}
