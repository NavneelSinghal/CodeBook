template <uint32_t SIEVE_MAX>
struct compact_sieve {
    constexpr compact_sieve() {
        uint32_t sqrt_mx = 0;
        while (sqrt_mx * sqrt_mx <= SIEVE_MAX) ++sqrt_mx;
        --sqrt_mx;
        uint32_t __max_prime = 3 + sqrt_mx;
        for (uint32_t i = 3; i <= __max_prime; i += 2)
            if (_raw_is_prime(i))
                for (uint32_t j = i * i; j < SIEVE_MAX; j += i << 1)
                    s[j >> 4] |= 1 << ((j >> 1) & 7);
    }
    unsigned char s[(SIEVE_MAX >> 4) + 47]{};
    constexpr bool _raw_is_prime(uint32_t n) const {
        return !(s[n >> 4] & (1 << ((n >> 1) & 7)));
    }
    constexpr bool is_prime(uint32_t n) const {
        return (n == 2) || _raw_is_prime(n);
    }
    vector<int> get_primes() const {
        vector<int> primes(1, 2);
        for (uint32_t i = 3; i < SIEVE_MAX; i += 2)
            if (_raw_is_prime(i)) primes.push_back(i);
        return primes;
    }
};
