// 1-based indexing
template <typename T>
struct Fenwick {
    int n;
    vector<T> t;
    Fenwick(int n) : n(n), t(n + 1) {}
    // prefix_sum[0..i]
    T query(int i) {
        T s = 0;
        while (i) {
            s += t[i];
            i -= i & (-i);
        }
        return s;
    }
    // range query
    T query(int l, int r) { return query(r) - query(l - 1); }
    // increase a[i] by v
    void update(int i, T v) {
        while (i <= n) {
            t[i] += v;
            i += i & (-i);
        }
    }
    // if next = true, returns index of first element that is > or >=
    // if next = false, returns index of last element that is < or <=
    // returns -1 if not found
    template <bool next, bool strict>
    int search(T v) {
        if (v == 0) {
            if constexpr (!next)
                return -int(strict);
            else if constexpr (!strict)
                return 0;
        } else if (v < 0) {
            return int(next) - 1;
        }
        T sum = 0;
        int pos = 0;
        for (int i = __lg(n); i >= 0; --i) {
            if (pos + (1 << i) <= n) {
                bool b = false;
                T s = sum + t[pos + (1 << i)];
                if constexpr (next != strict) {
                    b = s < v;
                } else {
                    b = s <= v;
                }
                if (b) {
                    sum = s;
                    pos += (1 << i);
                }
            }
        }
        if constexpr (next)
            return pos + 1;
        else
            return pos;
    }
};

