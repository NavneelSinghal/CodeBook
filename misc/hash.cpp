namespace hashing {

    // #define USE_AES 0
    /* append to top
    #define USE_AES 1
    #if USE_AES
        #pragma GCC target("aes")
        #include <immintrin.h>
    #endif
    */

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

