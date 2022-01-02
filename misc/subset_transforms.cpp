// sum over subsets dp - for every index i, it finds the sum of all f(j)
// where j is a subset of i in binary
template <typename T>
std::vector<T> zeta_transform(const std::vector<T>& v) {
    int n = (int)v.size();
    assert((n & (n - 1)) == 0);
    auto result{v};
    for (int i = 1; i < n; i <<= 1)
        for (int mask = 0; mask < n; ++mask)
            if (mask & i) result[mask] += result[mask ^ i];
    // to compute over supersets, just swap these two
    return result;
}

// mobius dp - inclusion exclusion, inverse of zeta transform
template <typename T>
std::vector<T> mobius_transform(const std::vector<T>& v) {
    int n = (int)v.size();
    assert((n & (n - 1)) == 0);
    auto result{v};
    for (int i = 1; i < n; i <<= 1)
        for (int mask = 0; mask < n; ++mask)
            if (mask & i) result[mask] -= result[mask ^ i];
    // to retrieve from superset sums instead, swap these two
}

// subset convolution
template <typename T>
std::vector<T> subset_convolution(const std::vector<T>& a,
                                  const std::vector<T>& b) {
    assert(a.size() == b.size());
    if (a.empty()) return std::vector<T>{};
    const int n = (int)a.size();
    assert((n & (n - 1)) == 0);
    const int lg_n = std::__lg(n);

    std::vector f_hat(lg_n + 1, std::vector<T>(n));
    auto g_hat{f_hat}, h{f_hat};

    for (int mask = 0; mask < n; mask++) {
        const int popcnt = __builtin_popcount(mask);
        f_hat[popcnt][mask] = a[mask];
        g_hat[popcnt][mask] = b[mask];
    }

    for (int i = 0; i <= lg_n; i++)
        for (int j = 0; j <= lg_n; j++)
            for (int mask = 0; mask < n; mask++)
                if ((mask & (1 << j)) != 0) {
                    f_hat[i][mask] += f_hat[i][mask ^ (1 << j)];
                    g_hat[i][mask] += g_hat[i][mask ^ (1 << j)];
                }

    for (int mask = 0; mask < n; mask++)
        for (int i = 0; i <= lg_n; i++)
            for (int j = 0; j <= i; j++)
                h[i][mask] += f_hat[j][mask] * g_hat[i - j][mask];

    for (int i = 0; i <= lg_n; i++)
        for (int j = 0; j <= lg_n; j++)
            for (int mask = 0; mask < n; mask++)
                if ((mask & (1 << j)) != 0) h[i][mask] -= h[i][mask ^ (1 << j)];

    std::vector<T> ans(n);
    for (int mask = 0; mask < n; mask++)
        ans[mask] = h[__builtin_popcount(mask)][mask];
    return ans;
}

