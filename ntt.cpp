#include <bits/stdc++.h>
using namespace std;

template <class T, class F = multiplies<T>>
T power(T a, long long n, F op = multiplies<T>(), T e = {1}) {
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
    M& operator+=(M r) {
        if ((v += r.v) >= Mod) v -= Mod;
        return *this;
    }
    M& operator-=(M r) {
        if ((v += Mod - r.v) >= Mod) v -= Mod;
        return *this;
    }
    M& operator*=(M r) {
        v = (uint64_t)v * r.v % Mod;
        return *this;
    }
    M& operator/=(M r) { return *this *= power(r, Mod - 2); }
    friend M operator+(M l, M r) { return l += r; }
    friend M operator-(M l, M r) { return l -= r; }
    friend M operator*(M l, M r) { return l *= r; }
    friend M operator/(M l, M r) { return l /= r; }
    friend bool operator==(M l, M r) { return l.v == r.v; }
    friend bool operator!=(M l, M r) { return l.v != r.v; }
    friend ostream& operator<<(ostream& os, M a) { return os << a.v; }
    friend istream& operator>>(istream& is, M& a) {
        int64_t w;
        is >> w;
        a = M(w);
        return is;
    }
};

using mint = Modular<998244353>;

namespace ntt {

    template <unsigned Mod>
        void ntt(vector<Modular<Mod>>& a, bool inverse) {
            static vector<Modular<Mod>> dw(30), idw(30);
            if (dw[0] == 0) {
                Modular<Mod> root = 2;
                while (power(root, (Mod - 1) / 2) == 1) root += 1;
                for (int32_t i = 0; i < 30; ++i)
                    dw[i] = -power(root, (Mod - 1) >> (i + 2)), idw[i] = 1 / dw[i];
            }
            int32_t n = a.size();
            assert((n & (n - 1)) == 0);
            if (not inverse) {
                for (int32_t m = n; m >>= 1;) {
                    Modular<Mod> w = 1;
                    int32_t m2 = m << 1;
                    for (int32_t s = 0, k = 0; s < n; s += m2) {
                        for (int32_t i = s, j = s + m; i < s + m; ++i, ++j) {
                            auto x = a[i], y = a[j] * w;
                            if (x.v >= Mod) x.v -= Mod;
                            a[i].v = x.v + y.v, a[j].v = x.v + (Mod - y.v);
                            // here a[i] is not normalised
                        }
                        w *= dw[__builtin_ctz(++k)];
                    }
                }
            } else {
                for (int32_t m = 1; m < n; m <<= 1) {
                    Modular<Mod> w = 1;
                    int32_t m2 = m << 1;
                    for (int32_t s = 0, k = 0; s < n; s += m2) {
                        for (int32_t i = s, j = s + m; i < s + m; ++i, ++j) {
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
            int32_t n = l.size(), m = r.size(), sz = 1 << __lg(((n + m - 1) << 1) - 1);
            if (min(n, m) < 30) {
                vector<long long> res(n + m - 1);
                for (int32_t i = 0; i < n; ++i)
                    for (int32_t j = 0; j < m; ++j) res[i + j] += (l[i] * r[j]).v;
                return {begin(res), end(res)};
            }
            bool eq = l == r;
            l.resize(sz), ntt(l, false);
            if (eq)
                r = l;
            else
                r.resize(sz), ntt(r, false);
            for (int32_t i = 0; i < sz; ++i) l[i] *= r[i];
            ntt(l, true), l.resize(n + m - 1);
            return l;
        }

    // for 1e9+7 ntt
    constexpr long long mod = 1e9 + 7;
    using Mint197 = Modular<mod>;

    vector<Mint197> operator*(const vector<Mint197>& l, const vector<Mint197>& r) {
        if (l.empty() or r.empty()) return {};
        int n = l.size(), m = r.size();
        static constexpr int mod0 = 998244353, mod1 = 1300234241, mod2 = 1484783617;
        using Mint0 = Modular<mod0>;
        using Mint1 = Modular<mod1>;
        using Mint2 = Modular<mod2>;
        vector<Mint0> l0(n), r0(m);
        vector<Mint1> l1(n), r1(m);
        vector<Mint2> l2(n), r2(m);
        for (int i = 0; i < n; ++i) l0[i] = l[i].v, l1[i] = l[i].v, l2[i] = l[i].v;
        for (int j = 0; j < m; ++j) r0[j] = r[j].v, r1[j] = r[j].v, r2[j] = r[j].v;
        l0 = l0 * r0, l1 = l1 * r1, l2 = l2 * r2;
        vector<Mint197> res(n + m - 1);
        static const Mint1 im0 = 1 / Mint1(mod0);
        static const Mint2 im1 = 1 / Mint2(mod1), im0m1 = im1 / mod0;
        static const Mint197 m0 = mod0, m0m1 = m0 * mod1;
        for (int i = 0; i < n + m - 1; ++i) {
            int y0 = l0[i].v;
            int y1 = (im0 * (l1[i] - y0)).v;
            int y2 = (im0m1 * (l2[i] - y0) - im1 * y1).v;
            res[i] = y0 + m0 * y1 + m0m1 * y2;
        }
        return res;
    }

}  // namespace ntt

using namespace ntt;

namespace IO {
    const int BUFFER_SIZE = 1 << 15;
    char input_buffer[BUFFER_SIZE];
    int input_pos = 0, input_len = 0;
    char output_buffer[BUFFER_SIZE];
    int output_pos = 0;
    char number_buffer[100];
    uint8_t lookup[100];
    void _update_input_buffer() {
        input_len = fread(input_buffer, sizeof(char), BUFFER_SIZE, stdin);
        input_pos = 0;
        if (input_len == 0) input_buffer[0] = EOF;
    }
    inline char next_char(bool advance = true) {
        if (input_pos >= input_len) _update_input_buffer();

        return input_buffer[advance ? input_pos++ : input_pos];
    }

    template <typename T>
        inline void read_int(T& number) {
            bool negative = false;
            number = 0;

            while (!isdigit(next_char(false)))
                if (next_char() == '-') negative = true;

            do {
                number = 10 * number + (next_char() - '0');
            } while (isdigit(next_char(false)));

            if (negative) number = -number;
        }

    template <typename T, typename... Args>
        inline void read_int(T& number, Args&... args) {
            read_int(number);
            read_int(args...);
        }

    void _flush_output() {
        fwrite(output_buffer, sizeof(char), output_pos, stdout);
        output_pos = 0;
    }

    inline void write_char(char c) {
        if (output_pos == BUFFER_SIZE) _flush_output();

        output_buffer[output_pos++] = c;
    }

    template <typename T>
        inline void write_int(T number, char after = '\0') {
            if (number < 0) {
                write_char('-');
                number = -number;
            }
            int length = 0;
            while (number >= 10) {
                uint8_t lookup_value = lookup[number % 100];
                number /= 100;
                number_buffer[length++] = (lookup_value & 15) + '0';
                number_buffer[length++] = (lookup_value >> 4) + '0';
            }
            if (number != 0 || length == 0) write_char(number + '0');
            for (int i = length - 1; i >= 0; i--) write_char(number_buffer[i]);
            if (after) write_char(after);
        }
    void IOinit() {
        // Make sure _flush_output() is called at the end of the program.
        bool exit_success = atexit(_flush_output) == 0;
        assert(exit_success);
        for (int i = 0; i < 100; i++) lookup[i] = (i / 10 << 4) + i % 10;
    }
}  // namespace IO

using namespace IO;

int32_t main() {
    IOinit();
    int n, m;
    read_int(n, m);
    vector<Mint197> a(n), b(m);
    for (auto& x : a) read_int(x);
    for (auto& x : b) read_int(x);
    a = a * b;
    for (int32_t i = 0; i < n + m - 1; ++i) {
        write_int(a[i].v, ' ');
    }
}
