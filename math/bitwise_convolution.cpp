enum convolution_type { AND, OR, XOR };

template <class T>
constexpr std::array<T, 4> transformation_matrix(convolution_type C) {
    if (C == AND)
        return {T(1), T(1), T(0), T(1)};
    else if (C == OR)
        return {T(1), T(1), T(1), T(0)};
    else
        return {T(1), T(1), T(1), T(-1)};
}

template <class T, convolution_type C>
void forward_bitwise_transform(std::vector<T>& pol) {
    constexpr auto T2 = transformation_matrix<T>(C);
    constexpr auto w = T2[0], x = T2[1], y = T2[2], z = T2[3];
    int n = (int)pol.size();
    for (int step = 1; step < n; step *= 2) {
        for (int pos = 0; pos < n; pos += step * 2) {
            for (int i = 0; i < step; ++i) {
                T a = pol[pos + i];
                T b = pol[pos + i + step];
                pol[pos + i] = w * a + x * b;
                pol[pos + i + step] = y * a + z * b;
            }
        }
    }
}

template <class T, convolution_type C,
          bool precompute_inverse_determinant = false>
void inverse_bitwise_transform(std::vector<T>& pol) {
    constexpr auto T2 = transformation_matrix<T>(C);
    constexpr auto w = T2[0], x = T2[1], y = T2[2], z = T2[3];
    constexpr auto determinant = w * z - x * y;
    constexpr auto inverse_determinant =
        precompute_inverse_determinant ? T(1) / determinant : T{};
    int n = (int)pol.size();
    for (int step = 1; step < n; step *= 2) {
        for (int pos = 0; pos < n; pos += step * 2) {
            for (int i = 0; i < step; ++i) {
                T a = pol[pos + i];
                T b = pol[pos + i + step];
                if constexpr (precompute_inverse_determinant) {
                    pol[pos + i] = (z * a - x * b) * inverse_determinant;
                    pol[pos + i + step] = (w * b - y * a) * inverse_determinant;
                } else {
                    pol[pos + i] = (z * a - x * b) / determinant;
                    pol[pos + i + step] = (w * b - y * a) / determinant;
                }
            }
        }
    }
}

template <class T, convolution_type C,
          bool precompute_inverse_determinant = false>
std::vector<T> bitwise_convolution(std::vector<T> a, std::vector<T> b) {
    assert(a.size() == b.size());
    if (a.empty()) return std::vector<T>{};
    assert(!(a.size() & (a.size() - 1)));
    forward_bitwise_transform<T, C>(a);
    forward_bitwise_transform<T, C>(b);
    int n = (int)a.size();
    for (int i = 0; i < n; ++i) a[i] *= b[i];
    inverse_bitwise_transform<T, C, precompute_inverse_determinant>(a);
    return a;
}

