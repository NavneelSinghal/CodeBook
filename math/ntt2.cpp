#include <bits/stdc++.h>
using namespace std;

using i32 = int_fast32_t;
using i64 = int_fast64_t;

// https://atcoder.jp/contests/abc169/tasks/abc169_f

namespace NTT {

// possible mods - 5 << 25, 7 << 26, 479 << 21, 483 << 21 with the same ROOT

const i64 MOD = (119 << 23) + 1;  // 998244353
const i64 ROOT = 62;

i64 modpow(i64 a, i64 n) {
    i64 ans = 1;
    while (n) {
        if (n & 1) ans = ans * a % MOD;
        a = a * a % MOD;
        n >>= 1;
    }
    return ans;
}

void ntt(vector<i64> &a) {
    i32 n = a.size();
    i32 L = 31 - __builtin_clz(n);

    static vector<i64> rt(2, 1);

    for (static i32 k = 2, s = 2; k < n; k <<= 1, ++s) {
        rt.resize(n);
        i64 z[] = {1, modpow(ROOT, MOD >> s)};
        for (int i = k; i < (k << 1); ++i) {
            rt[i] = rt[i >> 1] * z[i & 1] % MOD;
        }
    }

    vector<i32> rev(n);
    for (i32 i = 0; i < n; ++i) {
        rev[i] = (rev[i >> 1] | ((i & 1) << L)) >> 1;
    }

    for (i32 i = 0; i < n; ++i) {
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }

    for (i32 k = 1; k < n; k <<= 1) {
        for (i32 i = 0; i < n; i += (k << 1)) {
            for (i32 j = 0; j < k; ++j) {
                i64 z = rt[j + k] * a[i + j + k] % MOD;
                i64 &ai = a[i + j];
                a[i + j + k] = ai - z + ((z > ai) ? MOD : 0);
                ai += ((ai + z >= MOD) ? (z - MOD) : z);
            }
        }
    }
}

vector<i64> conv(const vector<i64> &a, const vector<i64> &b,
                 const i32 max_pow = -1) {
    if (a.empty() || b.empty()) return {};

    i32 s = a.size() + b.size() - 1, B = 32 - __builtin_clz(s), n = 1 << B,
        inv = modpow(n, MOD - 2);

    vector<i64> L(a), R(b), out(n);
    L.resize(n), R.resize(n);

    ntt(L), ntt(R);

    for (i32 i = 0; i < n; ++i) {
        out[(-i) & (n - 1)] = ((((i64)L[i] * R[i]) % MOD) * inv) % MOD;
    }

    ntt(out);

    if (max_pow >= 0) s = min(s, max_pow + 1);

    return {out.begin(), out.begin() + s};
}

}  // namespace NTT

int main() {
    i32 n, s;
    cin >> n >> s;

    vector<i32> a(n);
    for (auto &x : a) cin >> x;

    function<vector<i64>(i32, i32)> multiply_many = [&](i32 l, i32 r) {
        if (l > r) return vector<i64>(1, 1);
        if (l == r) {
            vector<i64> w(a[l] + 1);
            w[0] = 1;
            w[a[l]] = NTT::modpow(2, NTT::MOD - 2);
            return w;
        }
        int m = (l + r) >> 1;
        auto p = multiply_many(l, m);
        auto q = multiply_many(m + 1, r);
        return NTT::conv(p, q, s);
    };

    auto p = multiply_many(0, n - 1);

    cout << (NTT::modpow(2, n) * p[s]) % NTT::MOD << endl;
}
