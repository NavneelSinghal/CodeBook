// 1-based indexing - check
struct Fenwick {
    int n;
    vector<int> t;
    Fenwick(int n) : n(n), t(n + 1) {}

    // prefix_sum[0..i]
    int query(int i) {
        int s = 0;
        while (i) {
            s += t[i];
            i -= i & (-i);
        }
        return s;
    }

    // increase a[i] by v
    void update(int v, int i) {
        while (i <= n) {
            t[i] += v;
            i += i & (-i);
        }
    }
};
