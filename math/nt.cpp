#pragma GCC optimize("Ofast")
#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

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
    friend ostream& operator<<(ostream& os, M& a) { return os << a.v; }
    friend istream& operator>>(istream& is, M& a) {
        int64_t w;
        is >> w;
        a = M(w);
        return is;
    }
};

const unsigned mod = 1e9 + 7;

using mint = Modular<>;

// factorial precomputation

const int maxn = 1e5;

vector<mint> fact(maxn);
void precompute_facts() {
    fact[0] = 1;
    for (int32_t i = 0; i < maxn - 1; i++) {
        fact[i + 1] = fact[i] * (i + 1);
    }
}

mint C(int32_t n, int32_t k) { return fact[n] / (fact[k] * fact[n - k]); }

mint P(int32_t n, int32_t k) { return fact[n] / fact[n - k]; }

// O(1) square root

inline int64_t isqrt(int64_t n) {
    // long double ideally
    double N = n;
    N = sqrtl(N);
    long long sq = N - 2;
    sq = max(sq, 0LL);
    while (sq * sq < n) ++sq;
    if (sq * sq == n) return sq;
    return sq - 1;
}

namespace primeCount {
// https://en.wikipedia.org/wiki/Prime-counting_function
const int32_t Maxn = 1e7 + 10;
const int32_t MaxPrimes = 1e6 + 10;
const int32_t PhiN = 1e5;
const int32_t PhiK = 100;

// uint32_t ar[(Maxn >> 6) + 5] = {0};
int32_t len = 0;  // num of primes
int32_t primes[MaxPrimes];
int32_t pi[Maxn];
int32_t dp[PhiN][PhiK];
bitset<Maxn> fl;

void sieve(int32_t n) {
    fl[1] = true;
    for (int32_t i = 4; i <= n; i += 2) fl[i] = true;
    for (int32_t i = 3; i * i <= n; i += 2) {
        if (!fl[i]) {
            for (int32_t j = i * i; j <= n; j += i << 1) fl[j] = true;
        }
    }
    for (int32_t i = 1; i <= n; i++) {
        if (!fl[i]) primes[len++] = i;
        pi[i] = len;
    }
}

void init() {
    sieve(Maxn - 1);
    int32_t n, k, res;
    for (n = 0; n < PhiN; ++n) dp[n][0] = n;
    for (k = 1; k < PhiK; ++k) {
        int32_t p = primes[k - 1];
        for (n = 0; n < PhiN; ++n) {
            dp[n][k] = dp[n][k - 1] - dp[n / p][k - 1];
        }
    }
}

// for sum of primes, multiply the subtracted term by primes[k - 1] in both this
// and dp
int64_t non_multiples(int64_t n, int32_t k) {
    if (n < PhiN && k < PhiK) return dp[n][k];
    if (k == 1) return ((++n) >> 1);
    if (primes[k - 1] >= n) return 1;
    return non_multiples(n, k - 1) - non_multiples(n / primes[k - 1], k - 1);
}

int64_t Legendre(int64_t n) {
    if (n < Maxn) return pi[n];
    int32_t lim = sqrt(n) + 1;
    int32_t k = upper_bound(primes, primes + len, lim) - primes;
    return non_multiples(n, k) + k - 1;
}

// complexity: n^(2/3) (log n^(1/3))
// Lehmer's method to calculate pi(n)
int64_t Lehmer(int64_t n) {
    if (n < Maxn) return pi[n];
    int64_t w, res = 0;
    int32_t i, j, a, b, c, lim;
    b = sqrt(n), c = cbrt(n), a = Lehmer(sqrt(b)), b = Lehmer(b);
    res = non_multiples(n, a) + (((b + a - 2) * (b - a + 1)) >> 1);
    for (i = a; i < b; ++i) {
        w = n / primes[i];
        lim = Lehmer(sqrt(w)), res -= Lehmer(w);
        if (i <= c) {
            for (j = i; j < lim; ++j) {
                res += j;
                res -= Lehmer(w / primes[j]);
            }
        }
    }
    return res;
}
}  // namespace primeCount

// https://www.ams.org/journals/mcom/1996-65-213/S0025-5718-96-00674-6/S0025-5718-96-00674-6.pdf
// idk if this is the same resource but kinda similar complexity
// takes 170ms to compute pi(10^11)
namespace fastPrimeCount {
inline int64_t isqrt(int64_t n) {
    // long double ideally
    double N = n;
    N = sqrtl(N);
    int64_t sq = N - 2;
    sq = max(sq, (int64_t)0);
    while (sq * sq < n) ++sq;
    if (sq * sq == n) return sq;
    return sq - 1;
}
int64_t prime_pi(const int64_t N) {
    if (N <= 1) return 0;
    if (N == 2) return 1;
    const int32_t v = isqrt(N);
    int32_t s = (v + 1) / 2;
    vector<int32_t> smalls(s);
    for (int32_t i = 1; i < s; ++i) smalls[i] = i;
    vector<int32_t> roughs(s);
    for (int32_t i = 0; i < s; ++i) roughs[i] = 2 * i + 1;
    vector<int64_t> larges(s);
    for (int32_t i = 0; i < s; ++i) larges[i] = (N / (2 * i + 1) - 1) / 2;
    vector<bool> skip(v + 1);
    const auto divide = [](int64_t n, int64_t d) -> int32_t {
        return (double)(n) / d;
    };
    const auto half = [](int32_t n) -> int32_t { return (n - 1) >> 1; };
    int32_t pc = 0;
    for (int32_t p = 3; p <= v; p += 2)
        if (!skip[p]) {
            int32_t q = p * p;
            if (int64_t(q) * q > N) break;
            skip[p] = true;
            for (int32_t i = q; i <= v; i += 2 * p) skip[i] = true;
            int32_t ns = 0;
            for (int32_t k = 0; k < s; ++k) {
                int32_t i = roughs[k];
                if (skip[i]) continue;
                int64_t d = int64_t(i) * p;
                larges[ns] = larges[k] -
                             (d <= v ? larges[smalls[d >> 1] - pc]
                                     : smalls[half(divide(N, d))]) +
                             pc;
                roughs[ns++] = i;
            }
            s = ns;
            for (int32_t i = half(v), j = ((v / p) - 1) | 1; j >= p; j -= 2) {
                int32_t c = smalls[j >> 1] - pc;
                for (int32_t e = (j * p) >> 1; i >= e; --i) smalls[i] -= c;
            }
            ++pc;
        }
    larges[0] += int64_t(s + 2 * (pc - 1)) * (s - 1) / 2;
    for (int32_t k = 1; k < s; ++k) larges[0] -= larges[k];
    for (int32_t l = 1; l < s; ++l) {
        int32_t q = roughs[l];
        int64_t M = N / q;
        int32_t e = smalls[half(M / q)] - pc;
        if (e < l + 1) break;
        int64_t t = 0;
        for (int32_t k = l + 1; k <= e; ++k)
            t += smalls[half(divide(M, roughs[k]))];
        larges[0] += t - int64_t(e - l) * (pc + l - 1);
    }
    return larges[0] + 1;
}
}  // namespace fastPrimeCount

// also works well - O(n ^ (2/3))
namespace altPrimeCount {

using i64 = long long;

int isqrt(i64 n) { return sqrtl(n); }

__attribute__((target("avx"), optimize("O3", "unroll-loops"))) i64 prime_pi(
    const i64 N) {
    if (N <= 1) return 0;
    if (N == 2) return 1;
    const int v = isqrt(N);
    int s = (v + 1) / 2;
    vector<int> smalls(s);
    for (int i = 1; i < s; ++i) smalls[i] = i;
    vector<int> roughs(s);
    for (int i = 0; i < s; ++i) roughs[i] = 2 * i + 1;
    vector<i64> larges(s);
    for (int i = 0; i < s; ++i) larges[i] = (N / (2 * i + 1) - 1) / 2;
    vector<bool> skip(v + 1);
    const auto divide = [](i64 n, i64 d) -> int { return double(n) / d; };
    const auto half = [](int n) -> int { return (n - 1) >> 1; };
    int pc = 0;
    for (int p = 3; p <= v; p += 2)
        if (!skip[p]) {
            int q = p * p;
            if (i64(q) * q > N) break;
            skip[p] = true;
            for (int i = q; i <= v; i += 2 * p) skip[i] = true;
            int ns = 0;
            for (int k = 0; k < s; ++k) {
                int i = roughs[k];
                if (skip[i]) continue;
                i64 d = i64(i) * p;
                larges[ns] = larges[k] -
                             (d <= v ? larges[smalls[d >> 1] - pc]
                                     : smalls[half(divide(N, d))]) +
                             pc;
                roughs[ns++] = i;
            }
            s = ns;
            for (int i = half(v), j = ((v / p) - 1) | 1; j >= p; j -= 2) {
                int c = smalls[j >> 1] - pc;
                for (int e = (j * p) >> 1; i >= e; --i) smalls[i] -= c;
            }
            ++pc;
        }
    larges[0] += i64(s + 2 * (pc - 1)) * (s - 1) / 2;
    for (int k = 1; k < s; ++k) larges[0] -= larges[k];
    for (int l = 1; l < s; ++l) {
        int q = roughs[l];
        i64 M = N / q;
        int e = smalls[half(M / q)] - pc;
        if (e < l + 1) break;
        i64 t = 0;
        for (int k = l + 1; k <= e; ++k)
            t += smalls[half(divide(M, roughs[k]))];
        larges[0] += t - i64(e - l) * (pc + l - 1);
    }
    return larges[0] + 1;
}
}  // namespace altPrimeCount

int main() {
    long long n;
    cin >> n;
    cout << altPrimeCount::prime_pi(n) << '\n';
}
