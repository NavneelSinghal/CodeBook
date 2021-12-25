namespace primality {

    // source:
    // https://github.com/dacin21/dacin21_codebook/blob/master/nt/integer_factorization.cpp

    template <typename T, typename SFINAE = void>
    struct bigger_type {};
    template <typename T>
    using bigger_type_t = typename bigger_type<T>::type;
    template <typename T>
    struct bigger_type<
        T, typename enable_if<is_integral<T>::value && is_signed<T>::value &&
                              sizeof(T) == 4>::type> {
        using type = long long;
    };
    template <typename T>
    struct bigger_type<
        T, typename enable_if<is_integral<T>::value && !is_signed<T>::value &&
                              sizeof(T) == 4>::type> {
        using type = unsigned long long;
    };
    template <typename T>
    struct bigger_type<
        T, typename enable_if<is_integral<T>::value && is_signed<T>::value &&
                              sizeof(T) == 8>::type> {
        using type = __int128;
    };
    template <typename T>
    struct bigger_type<
        T, typename enable_if<is_integral<T>::value && !is_signed<T>::value &&
                              sizeof(T) == 8>::type> {
        using type = unsigned __int128;
    };

    template <typename int_t = unsigned long long>
    struct Mod_Int {
        static constexpr inline int_t add(int_t const& a, int_t const& b,
                                          int_t const& mod) {
            int_t ret = a + b;
            if (ret >= mod) ret -= mod;
            return ret;
        }
        static constexpr inline int_t sub(int_t const& a, int_t const& b,
                                          int_t const& mod) {
            return add(a, mod - b);
        }
        static constexpr inline int_t mul(int_t const& a, int_t const& b,
                                          int_t const& mod) {
            return a * static_cast<bigger_type_t<int_t>>(b) % mod;
        }
        static constexpr inline int_t pow(int_t const& a, int_t const& b,
                                          int_t const& mod) {
            int_t ret = 1;
            int_t base = a;
            for (int i = 0; b >> i; ++i) {
                if ((b >> i) & 1) ret = mul(ret, base, mod);
                base = mul(base, base, mod);
            }
            return ret;
        }
    };

    template <typename T>
    constexpr bool miller_rabin_single(T const& x, T base) {
        static_assert(is_integral<T>::value);
        if (x < T(4)) return x > T(1);
        if (x % 2 == 0) return false;
        base %= x;
        if (base == 0) return true;

        T xm1 = x - 1;
        int j = 1;
        T d = xm1 / 2;
        while (d % 2 == 0) {  // could use __builtin_ctz
            d /= 2;
            ++j;
        }
        T t = Mod_Int<T>::pow(base, d, x);
        if (t == T(1) || t == T(xm1)) return true;
        for (int k = 1; k < j; ++k) {
            t = Mod_Int<T>::mul(t, t, x);
            if (t == xm1) return true;
            if (t <= 1) break;
        }
        return false;
    }

    template <typename T>
    constexpr bool miller_rabin_multi(T const&) {
        return true;
    }
    template <typename T, typename... S>
    constexpr bool miller_rabin_multi(T const& x, T const& base,
                                      S const&... bases) {
        static_assert(is_integral<T>::value);
        if (!miller_rabin_single(x, base)) return false;
        return miller_rabin_multi(x, bases...);
    }

    template <typename T>
    constexpr bool miller_rabin(T const& x) {
        static_assert(is_integral<T>::value);
        if (x < 316349281)
            return miller_rabin_multi(x, T(11000544), T(31481107));
        if (x < 4759123141ull) return miller_rabin_multi(x, T(2), T(7), T(61));
        return miller_rabin_multi(x, T(2), T(325), T(9375), T(28178), T(450775),
                                  T(9780504), T(1795265022));
    }

}  // namespace primality


