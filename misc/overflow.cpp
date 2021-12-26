template <typename T, typename U>
bool check_mult_overflow(T&& a, U&& b) {
    return __builtin_mul_overflow_p(a, b, (decltype(a + b))0);
}
template <typename T, typename U>
bool check_add_overflow(T&& a, U&& b) {
    return __builtin_add_overflow_p(a, b, (decltype(a + b))0);
}
template <typename T, typename U>
bool check_sub_overflow(T&& a, U&& b) {
    return __builtin_sub_overflow_p(a, b, (decltype(a + b))0);
}
