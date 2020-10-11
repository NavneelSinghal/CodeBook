#include <bits/stdc++.h>
using namespace std;

// https://atcoder.jp/contests/abc169/tasks/abc169_f

using i32 = int_fast32_t;
using i64 = int_fast64_t;
using u64 = uint_fast64_t;

template <class T, class F = multiplies<T>>
T pwr(T a, long long n, F op = multiplies<T>(), T e = {1}) {
    assert(n >= 0);
    T res = e;
    while (n) {
        if (n & 1) res = op(res, a);
        if (n >>= 1) a = op(a, a);
    }
    return res;
}

template <unsigned Mod = 998'244'353>
struct Modular {
    using M = Modular;
    unsigned v;
    Modular(long long a = 0) : v((a %= Mod) < 0 ? a + Mod : a) {}
    M operator-() const { return M() -= *this; }
    M& operator+=(M r) { if ((v += r.v) >= Mod) v -= Mod; return *this; }
    M& operator-=(M r) { if ((v += Mod - r.v) >= Mod) v -= Mod; return *this; }
    M& operator*=(M r) { v = (u64)v * r.v % Mod; return *this; }
    M& operator/=(M r) { return *this *= pwr(r, Mod - 2); }
    friend M operator+(M l, M r) { return l += r; }
    friend M operator-(M l, M r) { return l -= r; }
    friend M operator*(M l, M r) { return l *= r; }
    friend M operator/(M l, M r) { return l /= r; }
    friend bool operator==(M l, M r) { return l.v == r.v; }
    friend bool operator!=(M l, M r) { return l.v != r.v; }
    friend ostream& operator<<(ostream& os, M a) { return os << a.v; }
    friend istream& operator>>(istream& is, M& a) { i64 w; is >> w; a = M(w); return is; }
};

using mint = Modular<998'244'353>;

namespace ntt {

    template <unsigned Mod>
        void ntt(vector<Modular<Mod>>& a, bool inverse) {
            static vector<Modular<Mod>> dw(30), idw(30);
            if (dw[0] == 0) {
                Modular<Mod> root = 2;
                while (pwr(root, (Mod - 1) / 2) == 1) root += 1;
                for (i32 i = 0; i < 30; ++i)
                    dw[i] = -pwr(root, (Mod - 1) >> (i + 2)), idw[i] = 1 / dw[i];
            }
            i32 n = a.size();
            assert((n & (n - 1)) == 0);
            if (not inverse) {
                for (i32 m = n; m >>= 1;) {
                    Modular<Mod> w = 1;
                    i32 m2 = m << 1;
                    for (i32 s = 0, k = 0; s < n; s += m2) {
                        for (i32 i = s, j = s + m; i < s + m; ++i, ++j) {
                            auto x = a[i], y = a[j] * w;
                            if (x.v >= Mod) x.v -= Mod;
                            a[i].v = x.v + y.v, a[j].v = x.v + (Mod - y.v);
                            // here a[i] is not normalised
                        }
                        w *= dw[__builtin_ctz(++k)];
                    }
                }
            } else {
                for (i32 m = 1; m < n; m <<= 1) {
                    Modular<Mod> w = 1;
                    i32 m2 = m << 1;
                    for (i32 s = 0, k = 0; s < n; s += m2) {
                        for (i32 i = s, j = s + m; i < s + m; ++i, ++j) {
                            auto x = a[i], y = a[j];
                            a[i] = x + y, a[j].v = x.v + (Mod - y.v), a[j] *= w;
                        }
                        w *= idw[__builtin_ctz(++k)];
                    }
                }
                auto inv = 1 / Modular<Mod>(n);
                for (auto&& e : a) e *= inv;
            }
        }
    template <unsigned Mod>
        vector<Modular<Mod>> operator*(vector<Modular<Mod>> l, vector<Modular<Mod>> r) {
            if (l.empty() or r.empty()) return {};
            i32 n = l.size(), m = r.size(), sz = 1 << __lg(((n + m - 1) << 1) - 1);
            if (min(n, m) < 30) {
                vector<long long> res(n + m - 1);
                for (i32 i = 0; i < n; ++i)
                    for (i32 j = 0; j < m; ++j) res[i + j] += (l[i] * r[j]).v;
                return {begin(res), end(res)};
            }
            bool eq = l == r;
            l.resize(sz), ntt(l, false);
            if (eq)
                r = l;
            else
                r.resize(sz), ntt(r, false);
            for (i32 i = 0; i < sz; ++i) l[i] *= r[i];
            ntt(l, true), l.resize(n + m - 1);
            return l;
        }

    // for 1e9+7 ntt
    constexpr long long mod = 1e9 + 7;
    using Mint197 = Modular<mod>;

    vector<Mint197> operator*(const vector<Mint197>& l, const vector<Mint197>& r) {
        if (l.empty() or r.empty()) return {};
        i32 n = l.size(), m = r.size();
        static constexpr i32 mod0 = 998244353, mod1 = 1300234241, mod2 = 1484783617;
        using Mint0 = Modular<mod0>;
        using Mint1 = Modular<mod1>;
        using Mint2 = Modular<mod2>;
        vector<Mint0> l0(n), r0(m);
        vector<Mint1> l1(n), r1(m);
        vector<Mint2> l2(n), r2(m);
        for (i32 i = 0; i < n; ++i) l0[i] = l[i].v, l1[i] = l[i].v, l2[i] = l[i].v;
        for (i32 j = 0; j < m; ++j) r0[j] = r[j].v, r1[j] = r[j].v, r2[j] = r[j].v;
        l0 = l0 * r0, l1 = l1 * r1, l2 = l2 * r2;
        vector<Mint197> res(n + m - 1);
        static const Mint1 im0 = 1 / Mint1(mod0);
        static const Mint2 im1 = 1 / Mint2(mod1), im0m1 = im1 / mod0;
        static const Mint197 m0 = mod0, m0m1 = m0 * mod1;
        for (i32 i = 0; i < n + m - 1; ++i) {
            i32 y0 = l0[i].v;
            i32 y1 = (im0 * (l1[i] - y0)).v;
            i32 y2 = (im0m1 * (l2[i] - y0) - im1 * y1).v;
            res[i] = y0 + m0 * y1 + m0m1 * y2;
        }
        return res;
    }

}  // namespace ntt

using namespace ntt;

int main() {

    const i32 MOD = 998'244'353;

    i32 n, s;
    cin >> n >> s;
    
    vector<mint> a(n);
    for (auto &x : a) cin >> x;

    function<vector<mint>(i32, i32)> multiply_many = [&] (i32 l, i32 r) {
        if (l > r) return vector<mint>(1, 1);
        if (l == r) {
            vector<mint> w(a[l].v + 1);
            w[0] = 1;
            w[a[l].v] = pwr(mint(2), MOD - 2);
            return w;
        }
        i32 m = (l + r) >> 1;
        auto p = multiply_many(l, m);
        auto q = multiply_many(m + 1, r);
        auto x = p * q;
        return vector<mint>(x.begin(), x.begin() + min(s + 1, (i32)x.size()));
    };
    
    auto p = multiply_many(0, n - 1);
    
    cout << (pwr(mint(2), n) * p[s]) << endl;
}
