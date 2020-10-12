#include <iostream>
#include <algorithm>
#include <iomanip>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <numeric>
#include <bitset>
#include <cmath>
#include <random>

using namespace std;
static const int MOD = 1000000007;
using ll = long long;
using u32 = unsigned;
using u64 = unsigned long long;
using u128 = __uint128_t;
using i64 = int64_t;

namespace factorize {

    using namespace std;

    template<class T> constexpr T INF = numeric_limits<T>::max()/32*15+208;

    struct mod64 {
        u64 n;
        static u64 mod, inv, r2;
        mod64() : n(0) {}
        mod64(u64 x) : n(init(x)) {}
        static u64 init(u64 w) { return reduce(u128(w) * r2); }
        static void set_mod(u64 m) {
            mod = inv = m;
            for (int i = 0; i < 5; ++i) inv *= 2 - inv * m;
            r2 = -u128(m) % m;
        }
        static u64 reduce(u128 x) {
            u64 y = u64(x >> 64) - u64((u128(u64(x) * inv) * mod) >> 64);
            return ll(y) < 0 ? y + mod : y;
        };
        mod64& operator+=(mod64 x) { n += x.n - mod; if(ll(n) < 0) n += mod; return *this; }
        mod64 operator+(mod64 x) const { return mod64(*this) += x; }
        mod64& operator*=(mod64 x) { n = reduce(u128(n) * x.n);  return *this; }
        mod64 operator*(mod64 x) const { return mod64(*this) *= x; }
        u64 val() const { return reduce(n); }
    };

    u64 mod64::mod, mod64::inv, mod64::r2;

    bool suspect(u64 a, u64 s, u64 d, u64 n){
        if(mod64::mod != n) mod64::set_mod(n);
        mod64 x(1), xx(a), one(x), minusone(n-1);
        while(d > 0){
            if(d&1) x = x * xx;
            xx = xx * xx;
            d >>= 1;
        }
        if (x.n == one.n) return true;
        for (int r = 0; r < s; ++r) {
            if(x.n == minusone.n) return true;
            x = x * x;
        }
        return false;
    }

    template<class T>
        bool miller_rabin(T n){
            if (n <= 1 || (n > 2 && n % 2 == 0)) return false;
            u64 d = n - 1, s = 0;
            while (!(d&1)) {++s; d >>= 1;}
            static const u64 v[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
            static const u64 v_small[] = {2, 7, 61};
            if(n <= 4759123141LL){
                for (auto &&p : v_small) {
                    if(p >= n) break;
                    if(!suspect(p, s, d, n)) return false;
                }
            }else {
                for (auto &&p : v) {
                    if(p >= n) break;
                    if(!suspect(p, s, d, n)) return false;
                }
            }
            return true;
        }

    template<typename T>
        struct ExactDiv {
            T t, i, val;
            ExactDiv() {}
            ExactDiv(T n) : t(T(-1) / n), i(mul_inv(n)) , val(n) {};
            T mul_inv(T n) {
                T x = n;
                for (int j = 0; j < 5; ++j) x *= 2 - n * x;
                return x;
            }
            bool divide(T n) const {
                if(val == 2) return !(n & 1);
                return n * this->i <= this->t;
            }
        };

    vector<ExactDiv<u64>> get_prime(int n){
        if(n <= 1) return vector<ExactDiv<u64>>();
        vector<bool> is_prime(n+1, true);
        vector<ExactDiv<u64>> prime;
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i <= n; ++i) {
            if(is_prime[i]) prime.emplace_back(i);
            for (auto &&j : prime){
                if(i*j.val > n) break;
                is_prime[i*j.val] = false;
                if(j.divide(i)) break;
            }
        }
        return prime;
    }
    const auto primes = get_prime(50000);

    random_device rng;

    u64 Isqrt(u64 const &x){
        u64 ret = (u64)sqrtl(x);
        while(ret > 0 && ret*ret > x) --ret;
        while(x - ret*ret > 2*ret) ++ret;
        return ret;
    }

    // SQUFOF https://github.com/radii/msieve
    vector<u32> saved;
    u64 squfof_iter(u64 &x, u64 &k, u64 &it_max, u32 cutoff_div) {
        if (__gcd(k, x) != 1) return __gcd(k, x);
        saved.clear();
        u64 scaledn = k*x;
        if (scaledn >> 62) return 1;
        u32 sqrtn = Isqrt(scaledn), cutoff = Isqrt(2*sqrtn)/cutoff_div, q0 = 1, p1 = sqrtn, q1 = scaledn - p1*p1;
        if (q1 == 0) return __gcd(x, (u64)p1);
        u32 mult = 2*k, coarse_cutoff = cutoff*mult, p0 = 0, sqrtq = 0, i, j;
        for (i = 0; i < it_max; ++i) {
            u32 q = 1, bits = 0, tmp = sqrtn + p1 - q1;
            if (tmp >= q1) q += tmp / q1;
            p0 = q*q1 - p1, q0 += (p1-p0)*q;
            if (q1 < coarse_cutoff) {
                tmp = q1 / __gcd(q1, mult);
                if (tmp < cutoff) saved.emplace_back(tmp);
            }
            tmp = q0;
            while (!(tmp & 1)) bits++, tmp >>= 1;
            if (!(bits & 1) && ((tmp & 7) == 1)) {
                sqrtq = Isqrt(q0);
                if (sqrtq*sqrtq == q0) {
                    for (j = 0; j < saved.size(); ++j) if (saved[j] == sqrtq) break;
                    if (j == saved.size()) break; // not found
                }
            }
            tmp = sqrtn + p0 - q0, q = 1;
            if (tmp >= q0) q += tmp / q0;
            p1 = q * q0 - p0, q1 += (p0-p1)*q;
            if (q0 < coarse_cutoff) {
                tmp = q0 / __gcd(q0, mult);
                if (tmp < cutoff) saved.emplace_back(tmp);
            }
        }
        if (sqrtq == 1 || i == it_max) return 1;
        q0 = sqrtq, p1 = p0 + sqrtq*((sqrtn - p0)/sqrtq), q1 = (scaledn - (u64)p1*p1)/q0;
        for (j = 0; j < it_max; ++j) {
            u32 q = 1, tmp = sqrtn + p1 - q1;
            if (tmp >= q1) q += tmp / q1;
            p0 = q*q1 - p1, q0 += (p1 - p0)*q;
            if (p0 == p1) { q0 = q1; break; }
            q = 1, tmp = sqrtn + p0 - q0;
            if (tmp >= q0) q += tmp / q0;
            p1 = q*q0 - p0, q1 += (p0 - p1)*q;
            if (p0 == p1) break;
        }
        if (j == it_max) return 1; // random fail
        return  __gcd((u64)q0, x);
    }

    u64 SQUFOF(u64 n){
        constexpr array<u64, 16> multipliers = {1,3,5,7,11,15,21,33,35,55,77,105,165,231,385,1155};
        constexpr array<u64, 8> fact = {1, 4, 16, 64, 256, 1024, 4096, 16384};
        {
            u64 ok = 0, ng = 1000000001;
            while(ng-ok > 1){
                u64 mid = (ok+ng)/2; (mid*mid <= n ? ok : ng) = mid;
            }
            if(ok*ok == n) return ok;
            ng = 1000001, ok = 0;
            while(ng-ok > 1){
                u64 mid = (ok+ng)/2; (mid*mid <= n ? ok : ng) = mid;
            }
            if(ok*ok*ok == n) return ok;
        }
        for (auto &&i : fact) {
            for (auto &&j : multipliers) {
                if(numeric_limits<u64>::max()/j <= n) continue;
                u64 it_max = 300*i; u64 jj = j;
                u64 val = squfof_iter(n, jj, it_max, 1);
                if(val != 1 && val != n) return val;
            }
        }
        return 1;
    }

    template<class T>
        T pollard_rho2(T n) {
            uniform_int_distribution<T> ra(1, n-1);
            if(mod64::mod != n) mod64::set_mod(n);
            while(true){
                u64 c_ = ra(rng), g = 1, r = 1, m = 500;
                while(c_ == n-2) c_ = ra(rng);
                mod64 y(ra(rng)), xx(0), c(c_), ys(0), q(1);
                while(g == 1){
                    xx.n = y.n;
                    for (int i = 1; i <= r; ++i) {
                        y *= y; y += c;
                    }
                    T k = 0; g = 1;
                    while(k < r && g == 1){
                        for (int i = 1; i <= (m > (r-k) ? (r-k) : m); ++i) {
                            ys.n = y.n; y *= y; y += c;
                            u64 xxx = xx.val(), yyy = y.val();
                            q *= mod64(xxx > yyy ? xxx - yyy : yyy - xxx);
                        }
                        g = __gcd<ll>(q.val(), n);
                        k += m;
                    }
                    r *= 2;
                }
                if(g == n) g = 1;
                while (g == 1){
                    ys *= ys; ys += c;
                    u64 xxx = xx.val(), yyy = ys.val();
                    g = __gcd<ll>(xxx > yyy ? xxx - yyy : yyy - xxx, n);
                }
                if (g != n && miller_rabin(g)) return g;
            }
        }

    template<class T>
        vector<T> prime_factor(T n){
            vector<T> a, res;
            for (auto &&i : primes) {
                while (i.divide(n)){
                    res.emplace_back(i.val);
                    n /= i.val;
                }
            }
            if(miller_rabin(n)){
                res.emplace_back(n);
                n = 1;
            }
            if(n != 1){
                a.emplace_back(n);
                while(!a.empty()){
                    ll x = a.back(); a.pop_back();
                    if(miller_rabin(x)) res.emplace_back(x);
                    else {
                        ll factor = pollard_rho2(x);
                        if(factor == 1 || factor == x) factor = SQUFOF(x);
                        while(factor == 1 || factor == x) factor = pollard_rho2(x);
                        a.emplace_back(factor);
                        a.emplace_back(x/factor);
                    }
                }
            }
            sort(res.begin(),res.end());
            return res;
        }
}

int main() {
    int q;
    cin >> q;
    while(q--){
        ll x;
        cin >> x;
        auto a = factorize::prime_factor(x);
        printf("%zu", a.size());
        for (auto &&i : a) {
            printf(" %lld", i);
        }
        puts("");
    }
    return 0;
}
