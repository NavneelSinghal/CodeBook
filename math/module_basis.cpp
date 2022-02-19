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
 
// if mods are too large, try fixing overflow
template <typename T, bool is_prime = false>
struct ModuleBasis {
    using ModuleVector = std::vector<T>;
    using TT = bigger_type_t<T>;
 
    T mod;
    int dims;
    std::vector<ModuleVector> basis;
 
    constexpr ModuleBasis(T mod_, int dims_)
        : mod{mod_}, dims{dims_}, basis(dims, std::vector<T>(dims, 0)) {}
 
    static constexpr std::array<T, 3> ext_gcd(T a, T b) {
        T old_r = a, r = b, old_s = 1, s = 0, old_t = 0, t = 1;
        while (r != 0) {
            T quot = old_r / r;
            std::tie(old_r, r) = std::make_pair(r, old_r - quot * r);
            std::tie(old_s, s) = std::make_pair(s, old_s - quot * s);
            std::tie(old_t, t) = std::make_pair(t, old_t - quot * t);
        }
        if (old_r < 0) {
            old_r *= -1;
            old_s *= -1;
            old_t *= -1;
        }
        // gcd, coeff of a, coeff of b
        return {old_r, old_s, old_t};
    }
 
    template <bool unsafe = false>
    void add_vector(ModuleVector v, int i = 0) {
        if (i == dims) return;
        if constexpr (!unsafe)
            for (auto& x : v) {
                x %= mod;
                if (x < 0) x += mod;
                // assert(x >= 0 && x < mod);
            }
        for (; i < dims; ++i) {
            if (v[i] == 0) continue;
            auto& ai = basis[i];
            if constexpr (!is_prime) {
                const auto z = (ai[i] == 0) ? mod : ai[i];
                auto [g, s, t] = ext_gcd(v[i], z);
                s %= mod, t %= mod;
                if (s < 0) s += mod;
                if (t < 0) t += mod;
                T x = z / g;
                T y = mod - v[i] / g;
                for (int j = i; j < dims; ++j) {
                    // might overflow for very large mods
                    // to fix this, take mods for both summands separately
                    TT old_aij = ai[j];
                    TT old_vj = v[j];
                    ai[j] = (old_vj * s + old_aij * t) % mod;
                    v[j] = (old_vj * x + old_aij * y) % mod;
                }
            } else {
                if (ai[i] == 0) {
                    auto [g, s, t] =
                        ext_gcd(v[i], mod);  // g = 1 if mod is prime
                    s %= mod, t %= mod;
                    if (s < 0) s += mod;
                    if (t < 0) t += mod;
                    // assert(g == 1);
                    for (int j = 0; j < dims; ++j) ai[j] = (TT(v[j]) * s) % mod;
                    break;
                } else {
                    TT factor = v[i];
                    for (int j = 0; j < dims; ++j) {
                        v[j] -= (factor * ai[j]) % mod;
                        if (v[j] < 0) v[j] += mod;
                        // assert(v[j] >= 0 && v[j] < mod);
                    }
                }
            }
        }
    }
 
    bool inside_basis(ModuleVector v) {
        for (int i = 0; i < dims; ++i) {
            const auto& ai = basis[i];
            if (ai[i] != 0 && v[i] % ai[i] == 0) {
                TT factor = v[i] / ai[i];
                for (int j = 0; j < dims; ++j) {
                    v[j] -= (ai[j] * factor) % mod;
                    if (v[j] < 0) v[j] += mod;
                    // assert(v[j] >= 0 && v[j] < mod);
                }
            }
            if (v[i] != 0) return false;
        }
        return true;
    }
 
    ModuleVector lexicographically_largest() {
        ModuleVector v(dims);
        for (int i = 0; i < dims; ++i) {
            const auto& ai = basis[i];
            if (ai[i] == 0) continue;
            // integer division
            TT lambda_i = (mod - 1 - v[i]) / ai[i];
            for (int j = 0; j < dims; ++j) {
                v[j] += (lambda_i * ai[j]) % mod;
                if (v[j] >= mod) v[j] -= mod;
                // assert(v[j] >= 0 && v[j] < mod);
            }
        }
        return v;
    }
 
    template <class CountType = unsigned long long>
    CountType count_representable_vectors() {
        CountType ans{1};
        for (int i = 0; i < dims; ++i) {
            auto aii = basis[i][i];
            if (aii == 0) continue;
            ans *= mod / aii;
        }
        return ans;
    }
};

