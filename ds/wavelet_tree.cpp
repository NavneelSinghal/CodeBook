template <class T>
struct wavelet_matrix {
    using size_type = uint32_t;
    struct bit_vector {
        static constexpr size_type wsize = 64;
        static size_type rank64(uint64_t x, size_type i) {
            return __builtin_popcountll(x & ((1ULL << i) - 1));
        }
#pragma pack(4)
        struct block_t {
            uint64_t bit;
            size_type sum;
        };
#pragma pack()
        size_type n, zeros;
        std::vector<block_t> block;
        bit_vector(size_type _n = 0) : n(_n), block(n / wsize + 1) {}
        int operator[](size_type i) const {
            return block[i / wsize].bit >> i % wsize & 1;
        }
        void set(size_type i) {
            block[i / wsize].bit |= (uint64_t)1 << i % wsize;
        }
        void build() {
            for (size_type j = 0; j < n / wsize; ++j)
                block[j + 1].sum =
                    block[j].sum + __builtin_popcountll(block[j].bit);
            zeros = rank0(n);
        }
        size_type rank0(size_type i) const { return i - rank1(i); }
        size_type rank1(size_type i) const {
            auto&& e = block[i / wsize];
            return e.sum + rank64(e.bit, i % wsize);
        }
    };
    size_type n, lg;
    std::vector<T> a;
    std::vector<bit_vector> bv;
    wavelet_matrix(size_type _n = 0) : n(_n), a(n) {}
    wavelet_matrix(const std::vector<T>& _a) : n(_a.size()), a(_a) { build(); }
    T& operator[](size_type i) { return a[i]; }
    void build() {
        lg = std::__lg(std::max<T>(
                 *std::max_element(std::begin(a), std::end(a)), 1)) +
             1;
        bv.assign(lg, n);
        std::vector<T> cur = a, nxt(n);
        for (auto h = lg; h--;) {
            for (size_type i = 0; i < n; ++i)
                if (cur[i] >> h & 1) bv[h].set(i);
            bv[h].build();
            std::array<decltype(std::begin(nxt)), 2> it{
                std::begin(nxt), std::begin(nxt) + bv[h].zeros};
            for (size_type i = 0; i < n; ++i) *it[bv[h][i]]++ = cur[i];
            std::swap(cur, nxt);
        }
    }
    // find kth element in [l, r), 0 indexed
    T kth(size_type l, size_type r, size_type k) const {
        T res = 0;
        for (auto h = lg; h--;) {
            auto l0 = bv[h].rank0(l), r0 = bv[h].rank0(r);
            if (k < r0 - l0)
                l = l0, r = r0;
            else {
                k -= r0 - l0;
                res |= (T)1 << h;
                l += bv[h].zeros - l0;
                r += bv[h].zeros - r0;
            }
        }
        return res;
    }
    // count i in [l..r) satisfying a[i] < ub
    size_type count(size_type l, size_type r, T ub) const {
        if (ub >= (T)1 << lg) return r - l;
        if (ub <= 0) return 0;
        size_type res = 0;
        for (auto h = lg; h--;) {
            auto l0 = bv[h].rank0(l), r0 = bv[h].rank0(r);
            if (~ub >> h & 1)
                l = l0, r = r0;
            else {
                res += r0 - l0;
                l += bv[h].zeros - l0;
                r += bv[h].zeros - r0;
            }
        }
        return res;
    }
    // count i in [l..r) satisfying a[i] in [lb, ub)
    size_type count(size_type l, size_type r, T lb, T ub) const {
        return count(l, r, ub) - count(l, r, lb);
    }
};

template <class T>
auto zip(const std::vector<T>& a) {
    int n = size(a);
    std::vector<std::pair<T, int>> p(n);
    for (int i = 0; i < n; ++i) p[i] = {a[i], i};
    std::sort(std::begin(p), std::end(p));
    std::vector<int> na(n);
    std::vector<T> v;
    for (int k = 0, rnk = -1; k < n; ++k) {
        if (k == 0 or p[k - 1].first < p[k].first)
            v.push_back(p[k].first), ++rnk;
        na[p[k].second] = rnk;
    }
    return std::make_pair(na, v);
}

/*

usage sample

using namespace std;

int main() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto& x : a) cin >> x;
    auto [na, v] = zip(a);
    wavelet_matrix wm(na);
    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        cout << v[wm.kth(l, r, k)] << '\n';
    }
}

*/
