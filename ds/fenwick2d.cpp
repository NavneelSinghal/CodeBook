// 1-based indexing
template <class T>
struct Fenwick2D {
    int n, m;
    vector<vector<T>> t;
    Fenwick2D(int n, int m) : n(n), m(m), t(n + 1, vector<T>(m + 1)) {}
    T query(int i, int j) {
        T s = 0;
        for (int x = i; x > 0; x -= x & (-x))
            for (int y = j; y > 0; y -= y & (-y)) s += t[x][y];
        return s;
    }
    T query(int i1, int j1, int i2, int j2) {
        return query(i2, j2) - query(i2, j1 - 1) + query(i1 - 1, j1 - 1) -
               query(i1 - 1, j2);
    }
    void update(int i, int j, T v) {
        for (int x = i; x <= n; x += x & (-x))
            for (int y = j; y <= n; y += y & (-y)) t[x][y] += v;
    }
};

#include "ext/pb_ds/assoc_container.hpp"
#include "ext/pb_ds/tree_policy.hpp"

namespace hashing {
    using ull = std::uint64_t;
    static const ull FIXED_RANDOM =
        std::chrono::steady_clock::now().time_since_epoch().count();

    template <class T, class D = void>
    struct custom_hash {};

    // https://www.boost.org/doc/libs/1_55_0/doc/html/hash/combine.html
    template <class T>
    inline void hash_combine(ull& seed, const T& v) {
        custom_hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b97f4a7c15 + (seed << 12) + (seed >> 4);
    };

    // http://xorshift.di.unimi.it/splitmix64.c
    template <class T>
    struct custom_hash<
        T, typename std::enable_if<std::is_integral<T>::value>::type> {
        ull operator()(T v) const {
            ull x = v + 0x9e3779b97f4a7c15 + FIXED_RANDOM;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
            x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
            return x ^ (x >> 31);
        }
    };

    template <class T>
    struct custom_hash<T,
                       std::void_t<decltype(std::begin(std::declval<T>()))>> {
        ull operator()(const T& a) const {
            ull value = FIXED_RANDOM;
            for (auto& x : a) hash_combine(value, x);
            return value;
        }
    };

    template <class... T>
    struct custom_hash<std::tuple<T...>> {
        ull operator()(const std::tuple<T...>& a) const {
            ull value = FIXED_RANDOM;
            std::apply(
                [&value](T const&... args) {
                    (hash_combine(value, args), ...);
                },
                a);
            return value;
        }
    };

    template <class T, class U>
    struct custom_hash<std::pair<T, U>> {
        ull operator()(const std::pair<T, U>& a) const {
            ull value = FIXED_RANDOM;
            hash_combine(value, a.first);
            hash_combine(value, a.second);
            return value;
        }
    };

};  // namespace hashing

template <class T>
struct SparseFenwick2D {
    int n, m;
    __gnu_pbds::gp_hash_table<
        int, __gnu_pbds::gp_hash_table<int, T, hashing::custom_hash<int>>,
        hashing::custom_hash<int>>
        t;
    SparseFenwick2D(int n, int m) : n(n), m(m) {}
    T query(int i, int j) {
        T s = 0;
        for (int x = i; x > 0; x -= x & (-x)) {
            auto& tx = t[x];
            for (int y = j; y > 0; y -= y & (-y)) s += tx[y];
        }
        return s;
    }
    T query(int i1, int j1, int i2, int j2) {
        return query(i2, j2) - query(i2, j1 - 1) + query(i1 - 1, j1 - 1) -
               query(i1 - 1, j2);
    }
    void update(int i, int j, T v) {
        for (int x = i; x <= n; x += x & (-x)) {
            auto& tx = t[x];
            for (int y = j; y <= n; y += y & (-y)) tx[y] += v;
        }
    }
};


