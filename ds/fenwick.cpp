// 0-based indexing for API
// 1-based indexing for internals
template <typename T>
struct Fenwick {
    int n;
    vector<T> t;
    Fenwick(int n) : n(n), t(n + 1) {}
    // prefix sum [0, i)
    T query(int i) {
        T s{};
        while (i) {
            s += t[i];
            i -= i & (-i);
        }
        return s;
    }
    // range query [l, r)
    T query(int l, int r) { return query(r) - query(l); }
    // increase a[i] by v
    void update(int i, T v) {
        ++i;
        while (i <= n) {
            t[i] += v;
            i += i & (-i);
        }
    }
 
    // assumes f is monotonic - TT...TFF...F
    // returns the max r > 0 such that f(query([0, r)), r) is true
    // returns 0 if no such positive r exists
    template <class F>
    int max_right(F&& f) {
        // assert(f(0));
        T sum{};
        int pos = 0;
        for (int i = __lg(n); i >= 0; --i) {
            if (pos + (1 << i) <= n) {
                T s = sum + t[pos + (1 << i)];
                if (f(s, pos + (1 << i))) {
                    pos += (1 << i);
                    sum = s;
                }
            }
        }
        return pos;
    }
};

