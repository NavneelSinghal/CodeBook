// for more stuff: https://cp-algorithms.com/algebra/continued-fractions.html
// for sqrt of rationals: http://web.math.princeton.edu/mathlab/jr02fall/Periodicity/mariusjp.pdf

template <class T>
auto continued_fraction_sqrt(T n) {
    T x_0 = static_cast<T>(std::sqrt(n));
    assert(x_0 * x_0 < n);
    assert((x_0 + 1) * (x_0 + 1) > n);
    std::vector<T> repeating;
    // kth residue is (sqrt(n) + a_k) / b_k
    // 0 < a_k < sqrt(n)
    // 0 < b_k < 2 * sqrt(n)
    T a_1 = x_0, a_k = a_1;
    T b_1 = n - a_1 * a_1, b_k = b_1;
    do {
        T x_k = (x_0 + a_k) / b_k;  // floor division
        repeating.push_back(x_k);
        a_k = x_k * b_k - a_k;
        assert((n - a_k * a_k) % b_k == 0);
        b_k = (n - a_k * a_k) / b_k;
    } while (a_k != a_1 || b_k != b_1);
    assert(repeating.back() == 2 * x_0);
    return std::make_pair(x_0, repeating);
}
