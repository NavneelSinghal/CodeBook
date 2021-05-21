#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx,avx2,sse,sse2,sse3,sse4,popcnt")

#include "bits/stdc++.h"
#include "ext/pb_ds/assoc_container.hpp"
#include "ext/pb_ds/tree_policy.hpp"
// #include <ext/rope>

using namespace std;
using namespace __gnu_pbds;
// using namespace __gnu_cxx;

#ifdef DEBUG
    #include "../libs/misc/debug.hpp"
#endif
#ifdef TIMING
    #include "../libs/misc/timing.hpp"
#else
template <typename C = int, typename T1 = int, typename T2 = int>
struct Stopwatch {
    Stopwatch(const string& s) {}
    Stopwatch() {}
    void reset() {}
    void pause() {}
    void play() {}
    int64_t elapsed() { return 0; }
    void print() const {}
};
#endif

using ll = long long;
using ull = unsigned long long;
using ld = long double;

constexpr int inf = int(1e9);
constexpr ll linf = ll(3e18L);
constexpr ld eps = 1e-9;
ld pi = acosl(-1.0);

template <class T>
using min_heap = priority_queue<T, vector<T>, greater<T>>;

void setIO(string name = "") {
    cin.tie(nullptr)->sync_with_stdio(false);
    cin.exceptions(cin.failbit);
    cout << setprecision(20) << fixed;
    if (name.size() == 0) return;
    ignore = freopen((name + ".in").c_str(), "r", stdin);
    ignore = freopen((name + ".out").c_str(), "w", stdout);
}

auto main() -> signed {
#ifdef CUSTOM_IO
    IO::IOinit();
#else
    setIO();
#endif

    int TESTS = 1;
    // cin >> TESTS;

    auto precompute = [&]() -> void {
    };

    auto solve = [&](int) -> void {
        Stopwatch stopwatch;
    };

    auto brute = [&]() -> void {
    };

    ignore = brute;
    precompute();
    for (int _t = 1; _t <= TESTS; ++_t) {
        // cout << "Case #" << _t << ": ";
        solve(_t);
        // brute();
    }
    return 0;
}
