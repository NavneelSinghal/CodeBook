// O(1) square root

inline long long isqrt(long long n) {
    double N = n;
    N = sqrtl(N);
    long long sq = N - 2;
    sq = max(sq, 0LL);
    while (sq * sq < n) {
        sq++;
    }
    if ((sq * sq) == n) return sq;
    return sq - 1;
}

// kth roots from k = 1 to 64
// keep a global struct
struct KthRoot {
    using u32 = unsigned int;
    using u64 = unsigned long long int;
    vector<u64> pow[65]; // pow[k][x] = pow(x+2,k)  (k >= 4)
    KthRoot() {
        for (u32 t = 2; t < (1<<16); t++) {
            u64 s = t*t; s = s*s;
            for (int k = 4; ; k++) {
                pow[k].push_back(s);
                if (__builtin_umulll_overflow(s,t,&s)) break;
            }
        }
    }
    u64 sqrt(u64 n) const {
        if (n == -1ull) return u32(-1);
        u64 x = std::sqrt((double)n);
        return x*x > n ? x-1 : x;
    }
    u64 cbrt(u64 n) const {
        u64 x = 0, y = 0;
        for (int s = 63; s >= 0; s -= 3) {
            x <<= 1;
            y = 3*x*(x+1)+1;
            if (y <= (n>>s)) n -= y<<s, ++x;
        }
        return x;
    }
    u64 operator()(u64 n, int k) {
        assert(k >= 1);
        if (k == 1 || n == 0) return n;
        if (k == 2) return sqrt(n);
        if (k == 3) return cbrt(n);
        auto ub = upper_bound(pow[k].begin(), pow[k].end(), n);
        return (ub-pow[k].begin())+1;
    }
};

