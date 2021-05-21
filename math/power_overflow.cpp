template <typename T, typename U>
bool check_mult_overflow(T&& a, U&& b) {
    return __builtin_mul_overflow_p(a, b, (__typeof__(a + b))0);
}
template <typename T, typename U>
bool check_add_overflow(T&& a, U&& b) {
    return __builtin_add_overflow_p(a, b, (__typeof__(a + b))0);
}
template <typename T, typename U>
bool check_sub_overflow(T&& a, U&& b) {
    return __builtin_sub_overflow_p(a, b, (__typeof__(a + b))0);
}

const long long linf = 3e18;
long long power_overflow(long long a, long long n) {
    long long ans = 1;
    while (n) {
        if (n & 1) {
            if (ans < linf / a)
                ans *= a;
            else
                return linf;
        }
        if (n > 1) {
            if (a < linf / a)
                a *= a;
            else
                return linf;
        }
        n >>= 1;
    }
    return ans;
}

