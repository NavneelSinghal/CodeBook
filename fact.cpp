
namespace factorization {
    int64_t mult(int64_t a, int64_t b, int64_t mod) {
        return (__int128)a * b % mod;
    }
    int64_t f(int64_t x, int64_t c, int64_t mod) {
        return (mult(x, x, mod) + c) % mod;
    }
    // brent's algorithm in pollard's rho method
    int64_t find_factor(int64_t n, int64_t x0 = 2, int64_t c = 1) {
        int64_t x = x0, g = 1, q = 1, xs, y;
        int32_t m = 128, l = 1;
        while (g == 1) {
            y = x;
            for (int32_t i = 1; i < l; ++i) {
                x = f(x, c, n);
            }
            int32_t k = 0;
            while (k < l && g == 1) {
                xs = x;
                for (int32_t i = 0; i < m && i < l - k; ++i) {
                    x = f(x, c, n);
                    q = mult(q, abs(y - x), n);
                }
                g = __gcd(q, n);
                k += m;
            }
            l <<= 1;
        }
        if (g == n) {
            do {
                xs = f(xs, c, n);
                g = __gcd(n, abs(xs - y));
            } while ();
        }
        return g;
    }
    vector<int64_t> factors(int64_t n) {
        if (n == 1) return vector<int64_t>(1, 1);
    }
}  // namespace factorization
