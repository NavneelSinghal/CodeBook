// doesn't need idempotence, only associativity, like a segtree
// doesn't need identity either
template <typename T>
T combine(const T &a, const T &b) {
    return a + b;
}

template <class T, class Combine>
struct sparse_table {
    sparse_table(std::vector<T> a, const Combine& _combine)
        : combine(_combine) {
        n = (int)a.size();
        s = std::move(a);
        if (n == 1) return;
        int log_n = std::__lg(n - 1) + 1;
        s.resize(n * log_n, s[0]);
        int current_offset = n;
        for (int j = 2; j < n; j <<= 1, current_offset += n) {
            int stride = 2 * j;
            for (int k = 0; k < n; k += stride) {
                int middle = k + j;
                int next_k = std::min(k + stride, n);
                if (middle - 1 < n) {
                    s[current_offset + middle - 1] = s[middle - 1];
                    for (int l = middle - 2; l >= k; --l)
                        s[current_offset + l] =
                            combine(s[l], s[current_offset + l + 1]);
                }
                if (middle < n) {
                    s[current_offset + middle] = s[middle];
                    for (int l = middle + 1; l < next_k; ++l)
                        s[current_offset + l] =
                            combine(s[current_offset + l - 1], s[l]);
                }
            }
        }
    }
    // [l, r)
    T query(int l, int r) {
        assert(l < r && 0 <= l && r <= n);
        --r;
        if (l == r) return s[l];
        int base = std::__lg(l ^ r) * n;
        return combine(s[base + l], s[base + r]);
    }

   private:
    int n;
    std::vector<T> s;
    const Combine combine;
};

