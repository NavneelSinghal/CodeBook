// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx")
// #pragma GCC optimize("unroll-loops")

// clang-format off
#define BITSET64 0
#if BITSET64
    #include <string>
    #include <bits/functexcept.h>
    #include <iosfwd>
    #include <bits/cxxabi_forced.h>
    #include <bits/functional_hash.h>
    #pragma push_macro("__SIZEOF_LONG__")
    #pragma push_macro("__cplusplus")
    #define __SIZEOF_LONG__ __SIZEOF_LONG_LONG__
    #define unsigned unsigned long
    #define __cplusplus 201102L
    #define __builtin_popcountl __builtin_popcountll
    #define __builtin_ctzl __builtin_ctzll
    #include <bitset>
    #pragma pop_macro("__cplusplus")
    #pragma pop_macro("__SIZEOF_LONG__")
    #undef unsigned
    #undef __builtin_popcountl
    #undef __builtin_ctzl
#endif
// clang-format on

#include "bits/stdc++.h"
#include "ext/pb_ds/assoc_container.hpp"
#include "ext/pb_ds/tree_policy.hpp"
// #include <ext/rope>

// #define int int_fast32_t
// #define unsigned uint_fast32_t
// #define ll int_fast64_t
// #define ull uint_fast64_t
// #define ld long double
// #define ulll __uint128_t
// #define lll __int128

using ll = int_fast64_t;
using ull = uint_fast64_t;
using ld = long double;
using ulll = __uint128_t;
using lll = __int128;

template <typename T>
void ignore_unused(const T &) {}

namespace ExtendedIO {
#ifdef __SIZEOF_INT128__
    std::ostream &operator<<(std::ostream &os, __int128 const &value) {
        static char buffer[64];
        int index = 0;
        __uint128_t T = (value < 0) ? (-(value + 1)) + __uint128_t(1) : value;
        if (value < 0)
            os << '-';
        else if (T == 0)
            return os << '0';
        for (; T > 0; ++index) {
            buffer[index] = static_cast<char>('0' + (T % 10));
            T /= 10;
        }
        while (index > 0) os << buffer[--index];
        return os;
    }
    std::istream &operator>>(std::istream &is, __int128 &T) {
        static char buffer[64];
        is >> buffer;
        std::size_t len = strlen(buffer), index = 0;
        T = 0;
        int mul = 1;
        if (buffer[index] == '-') ++index, mul *= -1;
        for (; index < len; ++index)
            T = T * 10 + static_cast<int>(buffer[index] - '0');
        T *= mul;
        return is;
    }
#endif
}  // namespace ExtendedIO

using namespace ExtendedIO;

namespace Debug {

#define SFINAE(x, ...)             \
    template <class, class = void> \
    struct x : std::false_type {}; \
    template <class T>             \
    struct x<T, std::void_t<__VA_ARGS__>> : std::true_type {}

    SFINAE(DefaultIO, decltype(std::cout << std::declval<T &>()));
    SFINAE(IsTuple, typename std::tuple_size<T>::type);
    SFINAE(Iterable, decltype(std::begin(std::declval<T>())));

    template <class T>
    constexpr char Space(const T &) {
        return (Iterable<T>::value or IsTuple<T>::value) ? '\n' : ' ';
    }

    template <auto &os>
    struct Writer {
        template <class T>
        void Impl(T const &t) const {
            if constexpr (DefaultIO<T>::value)
                os << t;
            else if constexpr (Iterable<T>::value) {
                int i = 0;
                for (auto &&x : t)
                    ((i++) ? (os << Space(x), Impl(x)) : Impl(x));
            } else if constexpr (IsTuple<T>::value)
                std::apply(
                    [this](auto const &... args) {
                        int i = 0;
                        (((i++) ? (os << ' ', Impl(args)) : Impl(args)), ...);
                    },
                    t);
            else
                static_assert(IsTuple<T>::value, "No matching type for print");
        }
        template <class F, class... Ts>
        auto &operator()(F const &f, Ts const &... ts) const {
            return Impl(f), ((os << ' ', Impl(ts)), ...), os << '\n', *this;
        }
    };

    template <auto &is>
    struct Reader {
        template <class T>
        auto &Rd(T &t) const {
            if constexpr (DefaultIO<T>::value)
                is >> t;
            else if constexpr (Iterable<T>::value)
                for (auto &x : t) Rd(x);
            else if constexpr (IsTuple<T>::value)
                std::apply([this](auto &... args) { (Rd(args), ...); }, t);
            else
                static_assert(IsTuple<T>::value, "No matching type for read");
            return *this;
        }
        template <class T>
        auto operator()(T t) const {
            Rd(t);
            return t;
        }
    };

#ifdef DEBUG
    #define debug(args...)                               \
        {                                                \
            std::string _s = #args;                      \
            replace(_s.begin(), _s.end(), ',', ' ');     \
            std::stringstream _ss(_s);                   \
            std::istream_iterator<std::string> _it(_ss); \
            std::cerr << "Line " << __LINE__ << "\n";    \
            err(_it, args);                              \
        }

    void err(std::istream_iterator<std::string> it) { ignore_unused(it); }

    template <typename T, typename... Args>
    void err(std::istream_iterator<std::string> it, T a, Args... args) {
        std::cerr << "\033[0;31m" << *it << " = ";
        Writer<std::cerr>{}(a);
        std::cerr << "\033[0m";
        err(++it, args...);
    }

    #define ASSERT(...) \
        if (not(__VA_ARGS__)) throw runtime_error(#__VA_ARGS__)
#else
    #define debug(...) 0
    #define ASSERT(...) 0
#endif

    constexpr Writer<std::cout> print;
    constexpr Reader<std::cin> read;

}  // namespace Debug

using namespace Debug;

namespace CPPDS {

#ifdef PB_DS_ASSOC_CNTNR_HPP
    #define unordered_map gp_hash_table
#endif

#ifdef PB_DS_TREE_POLICY_HPP
    template <typename T>
    using ordered_set =
        __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less<T>,
                         __gnu_pbds::rb_tree_tag,
                         __gnu_pbds::tree_order_statistics_node_update>;
    template <typename T>
    using ordered_multiset =
        __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less_equal<T>,
                         __gnu_pbds::rb_tree_tag,
                         __gnu_pbds::tree_order_statistics_node_update>;
    template <class key, class value, class cmp = std::less<key>>
    using ordered_map =
        __gnu_pbds::tree<key, value, cmp, __gnu_pbds::rb_tree_tag,
                         __gnu_pbds::tree_order_statistics_node_update>;
    // find_by_order(k)  returns iterator to kth element starting from 0;
    // order_of_key(k) returns count of elements strictly smaller than k;
    // for ordered_multiset, lower_bound and upper_bound swap roles
#endif
    template <class T>
    using min_heap = std::priority_queue<T, std::vector<T>, std::greater<T>>;
}  // namespace CPPDS

using namespace CPPDS;

namespace Utility {

    template <class T, class U = T>
    bool ckmin(T &a, U &&b) {
        return b < a ? a = std::forward<U>(b), true : false;
    }

    template <class T, class U = T>
    bool ckmax(T &a, U &&b) {
        return a < b ? a = std::forward<U>(b), true : false;
    }

    template <typename T>
    inline T sq(T a) {
        return a * a;
    }

    template <typename T>
    inline T sq_dist(std::pair<T, T> &a, std::pair<T, T> &b) {
        return sq(a.first - b.first) + sq(a.second - b.second);
    }

    template <typename T>
    inline ld dist(std::pair<T, T> &a, std::pair<T, T> &b) {
        return sqrtl(sq_dist(a, b));
    }

    inline ll isqrt(ll n) {
        ll sq = (ll)sqrtl((ld)n) - 2;
        sq = std::max(sq, ll(0));
        while (sq * sq < n) sq++;
        if ((sq * sq) == n) return sq;
        return sq - 1;
    }

    inline bool is_sq(ll n) {
        ll w = isqrt(n);
        return w * w == n;
    }

    struct custom_hash {
        // http://xorshift.di.unimi.it/splitmix64.c
        static ull splitmix64(ull x) {
            x += 0x9e3779b97f4a7c15;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
            x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
            return x ^ (x >> 31);
        }

        size_t operator()(ull x) const {
            static const ull FIXED_RANDOM =
                std::chrono::steady_clock::now().time_since_epoch().count();
            return splitmix64(x + FIXED_RANDOM);
        }

        size_t operator()(std::pair<int, int> p) const {
            static const ull FIXED_RANDOM =
                std::chrono::steady_clock::now().time_since_epoch().count();
            return splitmix64(p.first * 31 + p.second + FIXED_RANDOM);
        }
    };

    template <class Fun>
    class y_combinator_result {
        Fun fun_;

       public:
        template <class T>
        explicit y_combinator_result(T &&fun) : fun_(std::forward<T>(fun)) {}
        template <class... Args>
        decltype(auto) operator()(Args &&... args) {
            return fun_(std::ref(*this), std::forward<Args>(args)...);
        }
    };

    template <class Fun>
    decltype(auto) y_combinator(Fun &&fun) {
        return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
    }

    template <typename T>
    struct Range {
        struct It {
            T i;
            const T skip;
            void operator++() { i += skip; }
            T operator*() const { return i; }
            bool operator!=(It o) const {
                return (skip >= 0) ? (i < *o) : (i > *o);
            }
        };
        const T l_, r_, skip_;
        Range(T l, T r, T skip = 1) : l_(l), r_(r), skip_(skip) {
#ifdef DEBUG
            assert(skip != 0);
#endif
        }
        Range(T n) : Range(T(0), n, T(1)) {}
        It begin() const {
            return (skip_ >= 0) ? It{l_, skip_} : It{r_ - 1, skip_};
        }
        It end() const {
            return (skip_ >= 0) ? It{r_, skip_} : It{l_ - 1, skip_};
        }
    };

    void setIO(std::string name = "") {
        std::ios_base::sync_with_stdio(0);
        std::cin.tie(0);
        std::cout.tie(0);
        std::cin.exceptions(std::cin.failbit);
        std::cout << std::setprecision(20) << std::fixed;
        if (name.size() == 0) return;
        FILE *fin = freopen((name + ".in").c_str(), "r", stdin);
        FILE *fout = freopen((name + ".out").c_str(), "w", stdout);
        ignore_unused(fin);
        ignore_unused(fout);
    }

    constexpr int mod = 1e9 + 7;
    constexpr int nttmod = 998244353;
    constexpr int inf = 1e9;
    constexpr ll linf = 3e18;
    constexpr ld eps = 1e-9;
    ld pi = acosl(-1.0);

    std::mt19937 rng(
        std::chrono::steady_clock::now().time_since_epoch().count());

    // when using integers, keep overflow in mind
    template <typename T>
    [[nodiscard]] T pwr(T a, ll n) {
        T ans(1);
        while (n) {
            if (n & 1) ans *= a;
            if (n > 1) a *= a;
            n >>= 1;
        }
        return ans;
    }

    template <typename I, typename P, bool b>
    I bin_search_split(I l, I r, const P &predicate) {
        --l, ++r;
        while (r - l > 1) {
            // auto mid = std::midpoint(l, r);
            auto mid = l + (r - l) / 2;
            if (predicate(mid))
                r = mid;
            else
                l = mid;
        }
        if constexpr (b)
            return r;
        else
            return l;
    }

    // returns first i in [l, r], p(i) true, and if none found, returns r + 1
    template <typename I, typename P>
    I first_true(I l, I r, const P &p) {
        return bin_search_split<I, P, true>(l, r, p);
    }

    // returns last i in [l, r], p(i) false, and if none found, returns l - 1
    template <typename I, typename P>
    I last_false(I l, I r, const P &p) {
        return bin_search_split<I, P, false>(l, r, p);
    }

}  // namespace Utility

using namespace Utility;

/* structs start */

template <uint32_t Modulus>
class Modular {
    using M = Modular;

   public:
    static_assert(int(Modulus) >= 1, "Modulus must be in the range [1, 2^31)");
    static constexpr int modulus() { return Modulus; }
    static M raw(uint32_t v) { return *reinterpret_cast<M *>(&v); }
    Modular() : v_(0) {}
    Modular(int64_t v) : v_((v %= Modulus) < 0 ? v + Modulus : v) {}
    template <class T>
    explicit operator T() const {
        return v_;
    }
    M &operator++() { return v_ = ++v_ == Modulus ? 0 : v_, *this; }
    M &operator--() { return --(v_ ? v_ : v_ = Modulus), *this; }
    M &operator*=(M o) { return v_ = uint64_t(v_) * o.v_ % Modulus, *this; }
    M &operator/=(M o) {
        auto [inv, gcd] = extgcd(o.v_, Modulus);
        assert(gcd == 1);
        return *this *= inv;
    }
    M &operator+=(M o) {
        return v_ = int(v_ += o.v_ - Modulus) < 0 ? v_ + Modulus : v_, *this;
    }
    M &operator-=(M o) {
        return v_ = int(v_ -= o.v_) < 0 ? v_ + Modulus : v_, *this;
    }
    friend M operator++(M &a, int) { return exchange(a, ++M(a)); }
    friend M operator--(M &a, int) { return exchange(a, --M(a)); }
    friend M operator+(M a) { return a; }
    friend M operator-(M a) { return a.v_ = a.v_ ? Modulus - a.v_ : 0, a; }
    friend M operator*(M a, M b) { return a *= b; }
    friend M operator/(M a, M b) { return a /= b; }
    friend M operator+(M a, M b) { return a += b; }
    friend M operator-(M a, M b) { return a -= b; }
    friend std::istream &operator>>(std::istream &is, M &x) {
        int64_t v;
        return is >> v, x = v, is;
    }
    friend std::ostream &operator<<(std::ostream &os, M x) {
        return os << x.v_;
    }
    friend bool operator==(M a, M b) { return a.v_ == b.v_; }
    friend bool operator!=(M a, M b) { return a.v_ != b.v_; }

   private:
    static std::pair<int, int> extgcd(int a, int b) {
        std::array<int, 2> x{1, 0};
        while (b) std::swap(x[0] -= a / b * x[1], x[1]), std::swap(a %= b, b);
        return {x[0], a};
    }
    uint32_t v_;
};

using mint = Modular<mod>;

/* structs end */

using namespace std;
using namespace __gnu_pbds;
// using namespace __gnu_cxx;

/* main code starts */

auto main() -> signed {
    setIO();
    int TESTS = 1;
    // cin >> TESTS;

    auto precompute = [&]() -> void {
    };

    auto solve = [&](int t) -> void {
        ignore_unused(t);
    };

    auto brute = [&]() -> void {
    };

    ignore_unused(brute);
    precompute();
    for (int _t = 1; _t <= TESTS; ++_t) {
        // cout << "Case #" << _t << ": ";
        solve(_t);
        // brute();
    }
    return 0;
}
