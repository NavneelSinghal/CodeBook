namespace hashing {

    using ll = std::int64_t;
    using ull = std::uint64_t;
    static const ull FIXED_RANDOM =
        std::chrono::steady_clock::now().time_since_epoch().count();

#if USE_AES
    std::mt19937 rd(FIXED_RANDOM);
    const __m128i KEY1{(ll)rd(), (ll)rd()};
    const __m128i KEY2{(ll)rd(), (ll)rd()};
#endif

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
        ull operator()(T _x) const {
            ull x = _x;
#if USE_AES
            // implementation defined till C++17, defined from C++20
            __m128i m{ll(ull(x) * 0xbf58476d1ce4e5b9ULL), (ll)FIXED_RANDOM};
            __m128i y = _mm_aesenc_si128(m, KEY1);
            __m128i z = _mm_aesenc_si128(y, KEY2);
            return z[0];
#else
            x += 0x9e3779b97f4a7c15 + FIXED_RANDOM;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
            x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
            return x ^ (x >> 31);
#endif
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

#include "ext/pb_ds/assoc_container.hpp"
#include "ext/pb_ds/tree_policy.hpp"

namespace pbds {
    using namespace __gnu_pbds;
#ifdef PB_DS_ASSOC_CNTNR_HPP
    template <class Key, class Value, class Hash>
    using unordered_map = gp_hash_table<
        Key, Value, Hash, std::equal_to<Key>, direct_mask_range_hashing<>,
        linear_probe_fn<>,
        hash_standard_resize_policy<hash_exponential_size_policy<>,
                                    hash_load_check_resize_trigger<>, true>>;
    template <class Key, class Hash>
    using unordered_set = pbds::unordered_map<Key, null_type, Hash>;
#endif
#ifdef PB_DS_TREE_POLICY_HPP
    template <typename T>
    using ordered_set = tree<T, null_type, std::less<T>, rb_tree_tag,
                             tree_order_statistics_node_update>;
    template <typename T>
    using ordered_multiset = tree<T, null_type, std::less_equal<T>, rb_tree_tag,
                                  tree_order_statistics_node_update>;
    template <class Key, class Value, class Compare = std::less<Key>>
    using ordered_map = tree<Key, Value, Compare, rb_tree_tag,
                             tree_order_statistics_node_update>;
    // find_by_order(k)  returns iterator to kth element starting from 0;
    // order_of_key(k) returns count of elements strictly smaller than k;
    // for ordered_multiset, lower_bound and upper_bound swap roles
#endif
}  // namespace pbds

template <typename Signature, typename Lambda>
struct Cache;

template <typename ReturnType, typename... Args, typename Lambda>
struct Cache<ReturnType(Args...), Lambda> {
    template <typename... DummyArgs>
    ReturnType operator()(DummyArgs&&... args) {
        auto tied_args = std::tie(args...);
        auto it = memo.find(tied_args);
        if (it == memo.end()) {
            return memo[tied_args] = f(*this, std::forward<DummyArgs>(args)...);
        } else {
            return it->second;
        }
    }

    template <class _Lambda>
    Cache(std::tuple<>, _Lambda&& _f) : f(std::forward<_Lambda>(_f)) {}

    Lambda f;
    using TiedArgs = std::tuple<std::decay_t<Args>...>;
    pbds::unordered_map<TiedArgs, ReturnType, hashing::custom_hash<TiedArgs>>
        memo;
};

template <class Signature, class Lambda>
auto use_cache(Lambda&& f) {
    return Cache<Signature, Lambda>(std::tuple{}, std::forward<Lambda>(f));
}

/* usage example
int main() {
    int _tests = 1;
    // cin >> _tests;
    for (int _test = 1; _test <= _tests; ++_test) {
        // cout << "Case #" << _test << ": ";
        int n;
        cin >> n;
        vector<int> a(n);
        for (auto& x : a) cin >> x;
        auto solve =
            use_cache<int(int, int)>([&](auto&& self, int l, int r) -> int {
                int mn = *min_element(begin(a) + l, begin(a) + r + 1);
                int pred = -1;
                int ret = mn;
                for (int i = l; i <= r; i++) {
                    a[i] -= mn;
                    if (a[i] == 0) {
                        if (pred != -1) ret += self(pred, i - 1);
                        pred = -1;
                    } else if (pred == -1) {
                        pred = i;
                    }
                }
                if (pred != -1 && a[r] != 0) ret += self(pred, r);
                return min(ret, r - l + 1);
            });
        cout << solve(0, n - 1) << '\n';
    }
}
*/
