struct modinfo {
    uint32_t mod, root;
};

template <modinfo const& ref>
struct modular {
    static constexpr uint32_t const& mod = ref.mod;
    static modular root() { return modular(ref.root); }
    uint32_t v;
    modular(int64_t vv = 0) { s(vv % mod + mod); }
    modular& s(uint32_t vv) {
        v = vv < mod ? vv : vv - mod;
        return *this;
    }
    modular operator-() const { return modular() - *this; }
    modular& operator+=(const modular& rhs) { return s(v + rhs.v); }
    modular& operator-=(const modular& rhs) { return s(v + mod - rhs.v); }
    modular& operator*=(const modular& rhs) {
        v = uint64_t(v) * rhs.v % mod;
        return *this;
    }
    modular& operator/=(const modular& rhs) { return *this *= rhs.inv(); }
    modular operator+(const modular& rhs) const {
        return modular(*this) += rhs;
    }
    modular operator-(const modular& rhs) const {
        return modular(*this) -= rhs;
    }
    modular operator*(const modular& rhs) const {
        return modular(*this) *= rhs;
    }
    modular operator/(const modular& rhs) const {
        return modular(*this) /= rhs;
    }
    modular pow(int n) const {
        modular res(1), x(*this);
        while (n) {
            if (n & 1) res *= x;
            x *= x;
            n >>= 1;
        }
        return res;
    }
    modular inv() const { return pow(mod - 2); }
    friend modular operator+(int x, const modular& y) { return modular(x) + y; }
    friend modular operator-(int x, const modular& y) { return modular(x) - y; }
    friend modular operator*(int x, const modular& y) { return modular(x) * y; }
    friend modular operator/(int x, const modular& y) { return modular(x) / y; }
    friend ostream& operator<<(ostream& os, const modular& m) {
        return os << m.v;
    }
    friend istream& operator>>(istream& is, modular& m) {
        int64_t x;
        is >> x;
        m = modular(x);
        return is;
    }
    bool operator<(const modular& r) const { return v < r.v; }
    bool operator==(const modular& r) const { return v == r.v; }
    bool operator!=(const modular& r) const { return v != r.v; }
    explicit operator bool() const { return v; }
};

template <class mint>
void inplace_fmt(vector<mint>& f, bool inv) {
    const int n = int(f.size());
    static vector<mint> g, ig, p2;
    if (int(g.size()) == 0) {
        for (int i = int(0); i < int(30); i++) {
            mint w = -mint::root().pow(((mint::mod - 1) >> (i + 2)) * 3);
            g.push_back(w);
            ig.push_back(w.inv());
            p2.push_back(mint(1 << i).inv());
        }
    }
    static constexpr uint32_t mod2 = mint::mod * 2;
    if (!inv) {
        int second = n;
        if (second >>= 1) {
            for (int i = int(0); i < int(second); i++) {
                uint32_t x = f[i + second].v;
                f[i + second].v = f[i].v + mint::mod - x;
                f[i].v += x;
            }
        }
        if (second >>= 1) {
            mint p = 1;
            for (int i = 0, k = 0; i < n; i += second * 2) {
                for (int j = int(i); j < int(i + second); j++) {
                    uint32_t x = (f[j + second] * p).v;
                    f[j + second].v = f[j].v + mint::mod - x;
                    f[j].v += x;
                }
                p *= g[__builtin_ctz(++k)];
            }
        }
        while (second) {
            if (second >>= 1) {
                mint p = 1;
                for (int i = 0, k = 0; i < n; i += second * 2) {
                    for (int j = int(i); j < int(i + second); j++) {
                        uint32_t x = (f[j + second] * p).v;
                        f[j + second].v = f[j].v + mint::mod - x;
                        f[j].v += x;
                    }
                    p *= g[__builtin_ctz(++k)];
                }
            }
            if (second >>= 1) {
                mint p = 1;
                for (int i = 0, k = 0; i < n; i += second * 2) {
                    for (int j = int(i); j < int(i + second); j++) {
                        uint32_t x = (f[j + second] * p).v;
                        f[j].v = (f[j].v < mod2 ? f[j].v : f[j].v - mod2);
                        f[j + second].v = f[j].v + mint::mod - x;
                        f[j].v += x;
                    }
                    p *= g[__builtin_ctz(++k)];
                }
            }
        }
    } else {
        int second = 1;
        if (second < n / 2) {
            mint p = 1;
            for (int i = 0, k = 0; i < n; i += second * 2) {
                for (int j = int(i); j < int(i + second); j++) {
                    uint64_t x = f[j].v + mint::mod - f[j + second].v;
                    f[j].v += f[j + second].v;
                    f[j + second].v = x * p.v % mint::mod;
                }
                p *= ig[__builtin_ctz(++k)];
            }
            second <<= 1;
        }
        for (; second < n / 2; second <<= 1) {
            mint p = 1;
            for (int i = 0, k = 0; i < n; i += second * 2) {
                for (int j = int(i); j < int(i + second / 2); j++) {
                    uint64_t x = f[j].v + mod2 - f[j + second].v;
                    f[j].v += f[j + second].v;
                    f[j].v = (f[j].v) < mod2 ? f[j].v : f[j].v - mod2;
                    f[j + second].v = x * p.v % mint::mod;
                }
                for (int j = int(i + second / 2); j < int(i + second); j++) {
                    uint64_t x = f[j].v + mint::mod - f[j + second].v;
                    f[j].v += f[j + second].v;
                    f[j + second].v = x * p.v % mint::mod;
                }
                p *= ig[__builtin_ctz(++k)];
            }
        }
        if (second < n) {
            for (int i = int(0); i < int(second); i++) {
                uint32_t x = f[i + second].v;
                f[i + second].v = f[i].v + mod2 - x;
                f[i].v += x;
            }
        }
        mint z = p2[__lg(n)];
        for (int i = int(0); i < int(n); i++) f[i] *= z;
    }
}

namespace arbitrary_convolution {
    constexpr modinfo base0{1045430273, 3};
    constexpr modinfo base1{1051721729, 6};
    constexpr modinfo base2{1053818881, 7};
    using mint0 = modular<base0>;
    using mint1 = modular<base1>;
    using mint2 = modular<base2>;
    template <class t, class mint>
    vector<t> sub(const vector<mint>& x, const vector<mint>& y,
                  bool same = false) {
        int n = int(x.size()) + int(y.size()) - 1;
        int s = 1;
        while (s < n) s *= 2;
        vector<t> z(s);
        for (int i = int(0); i < int(int(x.size())); i++) z[i] = x[i].v;
        inplace_fmt(z, false);
        if (!same) {
            vector<t> w(s);
            for (int i = int(0); i < int(int(y.size())); i++) w[i] = y[i].v;
            inplace_fmt(w, false);
            for (int i = int(0); i < int(s); i++) z[i] *= w[i];
        } else {
            for (int i = int(0); i < int(s); i++) z[i] *= z[i];
        }
        inplace_fmt(z, true);
        z.resize(n);
        return z;
    }

    template <class mint>
    vector<mint> multiply(const vector<mint>& x, const vector<mint>& y,
                          bool same = false) {
        auto d0 = sub<mint0>(x, y, same);
        auto d1 = sub<mint1>(x, y, same);
        auto d2 = sub<mint2>(x, y, same);
        int n = int(d0.size());
        vector<mint> res(n);
        static const mint1 r01 = mint1(mint0::mod).inv();
        static const mint2 r02 = mint2(mint0::mod).inv();
        static const mint2 r12 = mint2(mint1::mod).inv();
        static const mint2 r02r12 = r02 * r12;
        static const mint w1 = mint(mint0::mod);
        static const mint w2 = w1 * mint(mint1::mod);
        for (int i = int(0); i < int(n); i++) {
            uint64_t first = d0[i].v;
            uint64_t second =
                (d1[i].v + mint1::mod - first) * r01.v % mint1::mod;
            uint64_t c = ((d2[i].v + mint2::mod - first) * r02r12.v +
                          (mint2::mod - second) * r12.v) %
                         mint2::mod;
            res[i].v = (first + second * w1.v + c * w2.v) % mint::mod;
        }
        return res;
    }

    template <class mint>
    vector<mint> inv(const vector<mint>& a, int n) {  // compute inverse mod x^n
        assert(a[0] != 0);
        vector<mint> r{mint(1) / a[0]};
        int sz = 1;
        while (sz < n) {
            auto other = multiply(multiply(r, r), a);
            for (int i = 0; i < (int)other.size(); ++i) {
                if (i < (int)r.size())
                    other[i] = 2 * r[i] - other[i];
                else
                    other[i] *= -1;
            }
            r = other;
            sz *= 2;
            while (r.size() > sz) r.pop_back();
        }
        while (r.size() > n) r.pop_back();
        return r;
    }

}  // namespace arbitrary_convolution

using arbitrary_convolution::inv;
using arbitrary_convolution::multiply;

constexpr modinfo base{1000000007, 0};
using mint = modular<base>;

