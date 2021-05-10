template <typename T>
T combine(const T &a, const T &b) {
    return std::min(a, b);
}

// introduced by: Pa.Nic
//
// doesn't need idempotence, only associativity, like a segtree
// doesn't need identity either
template <typename T>
T combine(const T &a, const T &b) {
    return a + b;
}

template <typename T>
struct sparse_table {
    sparse_table(std::vector<T> a) {
        n = (int)a.size();
        s = std::move(a);
        if (n == 1) return;
        int l = __lg(n - 1) + 1;
        s.resize(n * l, s[0]);
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
        int base = __lg(l ^ r) * n;
        return combine(s[base + l], s[base + r]);
    }
   private:
    int n;
    std::vector<T> s;
};

// with explanation
template <typename T>
struct sparse_table {
    sparse_table(std::vector<T> a) {
        n = (int)a.size();
        s = std::move(a);
        if (n == 1) return;
        int l = __lg(n - 1) + 1;
        s.resize(n * l, s[0]);
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
        // find x such that x is 2^k * w
        // [l, x - 1] and [x, r] are completely inside [x - 2^k, x - 1] and [x,
        // x + 2^k - 1]
        //
        // l >= x - 2^k r <= x + 2^k - 1
        //
        // l = 01001010010, r = 01010100111
        //
        // middle - 1 = 0100111111 middle     = 0101000000
        assert(l < r && 0 <= l && r <= n);
        --r;
        if (l == r) return s[l];
        int base = __lg(l ^ r) * n;
        return combine(s[base + l], s[base + r]);
    }

    // nothing in particular for the first row
    // left right left right left right left right
    // left       right      left       right

    // 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11' | 12' | 13' | 14' | 15'
    // 0   1 | 2   3 | 4   5 | 6   7 | 8   9 | 10   11' | 12'   13' | 14'   15'
    // 0   1   2   3 | 4   5   6   7 | 8   9   10   11' | 12'   13'   14'   15'
    // 0   1   2   3   4   5   6   7 | 8   9   10   11'   12'   13'   14'   15'

   private:
    int n;
    std::vector<T> s;
};

// template <typename T>
// struct sparse_table {
//     sparse_table(std::vector<T> a) {
//         n = (int)a.size();
//         s = std::move(a);
//         if (n == 1) {
//             N = 1;
//             return;
//         }
//         int l = __lg(n - 1) + 1;
//         N = 1 << l;
//         s.resize(N * l, s[0]);
//         int current_offset = N;
//         for (int j = 2; j < N; j <<= 1, current_offset += N) {
//             int stride = 2 * j;
//             for (int k = 0; k < N; k += stride) {
//                 int middle = k + j;
//                 int next_k = k + stride;
//                 s[current_offset + middle - 1] = s[middle - 1];
//                 s[current_offset + middle] = s[middle];
//                 for (int l = middle - 2; l >= k; --l)
//                     s[current_offset + l] =
//                         combine(s[l], s[current_offset + l + 1]);
//                 for (int l = middle + 1; l < next_k; ++l)
//                     s[current_offset + l] =
//                         combine(s[current_offset + l - 1], s[l]);
//             }
//         }
//     }
// 
//     // [l, r)
//     T query(int l, int r) {
//         // find x such that x is 2^k * w
//         // [l, x - 1] and [x, r] are completely inside
//         //
//         // [x - 2^k, x - 1] and [x, x + 2^k - 1]
//         //
//         // l >= x - 2^k r <= x + 2^k - 1
//         //
//         // l = 01001010010, r = 01010100111
//         //
//         // middle - 1 = 0100111111 middle     = 0101000000
//         assert(l < r && 0 <= l && r <= n);
//         --r;
//         if (l == r) return s[l];
//         int base = __lg(l ^ r) * N;
//         return combine(s[base + l], s[base + r]);
//     }
// 
//     // nothing in particular for the first row
//     // left right left right left right left right
//     // left       right      left       right
// 
//     // 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11' | 12' | 13' | 14' | 15'
//     // 0   1 | 2   3 | 4   5 | 6   7 | 8   9 | 10   11' | 12'   13' | 14'   15'
//     // 0   1   2   3 | 4   5   6   7 | 8   9   10   11' | 12'   13'   14'   15'
//     // 0   1   2   3   4   5   6   7 | 8   9   10   11'   12'   13'   14'   15'
// 
//    private:
//     int n, N;
//     std::vector<T> s;
// };

template <typename T>
struct sparse_table {
    int n, l;
    vector<vector<T>> s;
    sparse_table(vector<T>& a) {
        n = a.size();
        l = 2 + __lg(n + 1);
        s.assign(l, vector<T>(n));
        s[0] = a;
        for (int j = 0; j < l - 1; ++j) {
            for (int i = 0; i + (2 << j) <= n; ++i) {
                s[j + 1][i] = min(s[j][i], s[j][i + (1 << j)]);
            }
        }
    }
    // [l, r)
    T query(int l, int r) {
        int sz = __lg(r - l);
        return min(s[sz][l], s[sz][r - (1 << sz)]);
    }
};

// sparse table
const int N = 1e6 + 6;
const int Log = 26;

int sparse_table[Log][N];

void build_sparse_table(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n; ++i) {
        sparse_table[0][i] = a[i];
    }
    for (int j = 0; j < Log - 1; ++j) {
        for (int i = 0; i + (2 << j) <= n; ++i) {
            sparse_table[j + 1][i] =
                min(sparse_table[j][i], sparse_table[j][i + (1 << j)]);
        }
    }
}

//[l, r)
int query(int l, int r) {
    int sz = __lg(r - l);
    return min(sparse_table[sz][l], sparse_table[sz][r - (1 << sz)]);
}
