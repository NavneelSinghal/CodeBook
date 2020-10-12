#include <iostream>
#include <cassert>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;
using u64 = uint64_t;
using u128 = __uint128_t;
using i64 = int64_t;
using i128 = __int128_t;

i64 mult (i64 a, i64 b, i64 mod) { return (i128)a * b % mod; }
i64 f (i64 x, i64 c, i64 mod) { return (mult(x, x, mod) + c) % mod; }

i64 rho (i64 n, i64 x0 = 2, i64 c = 1) {
    
    if ((n & 1) == 0) return 2;
    i64 g = n;
   
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<i64> d(1, n - 1), e(2, n - 1);
    
    while (g == n) {
        i64 x0 = e(rng), c = d(rng), x = x0, y = x0;
        g = 1;
        while (g == 1) {
            x = f(x, c, n);
            y = f(y, c, n);
            y = f(y, c, n);
            g = __gcd(abs(x - y), n);
        }
    }
    
    return g;
}

u64 binpower(u64 base, u64 e, u64 mod) {
    u64 result = 1;
    base %= mod;
    while (e) {
        if (e & 1) result = (u128)result * base % mod;
        base = (u128)base * base % mod;
        e >>= 1;
    }
    return result;
}

bool check_composite(u64 n, u64 a, u64 d, int s) {
    u64 x = binpower(a, d, n);
    if (x == 1 || x == n - 1) return false;
    for (int r = 1; r < s; r++) {
        x = (u128)x * x % n;
        if (x == n - 1) return false;
    }
    return true;
};

/*
bool MillerRabin(u64 n) { // returns true if n is prime, else returns false.
    if (n < 2) return false;
    int r = 0;
    u64 d = n - 1;
    while ((d & 1) == 0) {
        d >>= 1;
        r++;
    }
    for (int a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (n == a) return true;
        if (check_composite(n, a, d, r)) return false;
    }
    return true;
}
*/

// this literally makes no difference

bool MillerRabin(u64 n) { // returns true if n is prime, else returns false.
    if (n < 2)
        return false;

    int r = 0;
    u64 d = n - 1;
    while ((d & 1) == 0) {
        d >>= 1;
        r++;
    }

    for (auto x : {2, 3, 5, 13, 19, 73, 193, 407521, 299210837}) {
        if (x == n) return true;
    }

    for (int a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        if (check_composite(n, a, d, r))
            return false;
    }
    return true;
}


vector<u64> factors(u64 n) {
    if (n == 1) return vector<u64>();
    if (MillerRabin(n)) {
        vector<u64> ret;
        ret.push_back(n);
        return ret;
    }
    int x = rho(n);
    auto v = factors(x);
    auto u = factors(n / x);
    for (auto g : v) u.push_back(g);
    return u;
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        i64 n;
        cin >> n;
        auto r = factors(n);
        sort(r.begin(), r.end());
        cout << r.size() << ' ';
        for (auto x : r) {
            cout << x << ' ';
        }
        cout << '\n';
    }
    return 0;
}
