template <typename T = int>
array<T, 3> ext_gcd(T a, T b) {
    T old_r = a, r = b, old_s = 1, s = 0, old_t = 0, t = 1;
    while (r != 0) {
        T quot = old_r / r;
        tie(old_r, r) = make_pair(r, old_r - quot * r);
        tie(old_s, s) = make_pair(s, old_s - quot * s);
        tie(old_t, t) = make_pair(t, old_t - quot * t);
    }
    if (old_r < 0) {
        old_r *= -1;
        old_s *= -1;
        old_t *= -1;
    }
    // gcd, coeff of a, coeff of b
    return {old_r, old_s, old_t};
}

