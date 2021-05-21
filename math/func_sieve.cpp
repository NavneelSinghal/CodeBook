// memory - n + n/(log n)
// how to compute a multiplicative function in o(n) for all values
template <int n = 1'000'000>
struct fast_sieve_func_optimized {
    vector<int> primes, func;
    vector<char> is_prime;
    fast_sieve_func_optimized() {
        is_prime.assign(n + 1, true);
        is_prime[0] = is_prime[1] = false;
        func.resize(n + 1);
        func[1] = 1;  // for multiplicative functions, it is either 0 or 1
        for (int i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                primes.push_back(i);
                // here you should handle the case of primes by updating func[i]
            }
            for (auto p : primes) {
                if (i * p > n) break;
                is_prime[i * p] = false;
                // update func[i * p] in the following branches
                // or maintain the smallest prime factor and check using that
                if (i % p == 0) {
                    // p divides i
                    // see how to update here
                    break;
                } else {
                    // p and i are coprime
                    // see how to update here
                }
            }
        }
    }
};


