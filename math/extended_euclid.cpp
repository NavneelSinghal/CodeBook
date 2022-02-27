template <typename T, typename SFINAE = void>
struct bigger_type {};
template <typename T>
using bigger_type_t = typename bigger_type<T>::type;
template <typename T>
struct bigger_type<T, typename std::enable_if<std::is_integral<T>::value &&
                                              std::is_signed<T>::value &&
                                              sizeof(T) == 4>::type> {
    using type = long long;
};
template <typename T>
struct bigger_type<T, typename std::enable_if<std::is_integral<T>::value &&
                                              !std::is_signed<T>::value &&
                                              sizeof(T) == 4>::type> {
    using type = unsigned long long;
};
template <typename T>
struct bigger_type<T, typename std::enable_if<std::is_integral<T>::value &&
                                              std::is_signed<T>::value &&
                                              sizeof(T) == 8>::type> {
    using type = __int128;
};
template <typename T>
struct bigger_type<T, typename std::enable_if<std::is_integral<T>::value &&
                                              !std::is_signed<T>::value &&
                                              sizeof(T) == 8>::type> {
    using type = unsigned __int128;
};

template <typename T = int>
array<T, 3> ext_gcd(T a, T b) {
    T old_r = a, r = b, old_s = 1, s = 0, old_t = 0, t = 1;
    while (r != 0) {
        bigger_type_t<T> quot = old_r / r;
        tie(old_r, r) = make_pair(r, T(old_r - quot * r));
        tie(old_s, s) = make_pair(s, T(old_s - quot * s));
        tie(old_t, t) = make_pair(t, T(old_t - quot * t));
    }
    if (old_r < 0) {
        old_r *= -1;
        old_s *= -1;
        old_t *= -1;
    }
    // gcd, coeff of a, coeff of b
    return {old_r, old_s, old_t};
}

