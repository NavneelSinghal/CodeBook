template <int N = 1'000'000, bool compute_primes = true>
struct fast_sieve {
    unsigned char __compact_sieve[(N >> 4) + 47];
    vector<int> primes;
    inline bool _raw_is_prime(int n) {
        return !(__compact_sieve[n >> 4] & (1 << ((n >> 1) & 7)));
    }
    inline bool is_prime(int n) { return (n == 2) || _raw_is_prime(n); }
    void fill_sieve() {
        int __max_prime = 3 + sqrt(N);
        for (int i = 3; i <= __max_prime; i += 2)
            if (_raw_is_prime(i)) {
                int j = i * i;
                while (j < N) {
                    __compact_sieve[j >> 4] |= 1 << ((j >> 1) & 7);
                    j += i << 1;
                }
            }
    }
    vector<int> get_primes() {
        vector<int> primes(1, 2);
        for (int i = 3; i < N; i += 2)
            if (_raw_is_prime(i)) primes.push_back(i);
        return primes;
    }
    fast_sieve() {
        fill_sieve();
        if (compute_primes) primes = get_primes();
    }
};

