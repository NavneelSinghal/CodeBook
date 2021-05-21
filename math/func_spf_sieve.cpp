// memory - 3n + n/(log n)
template <int n = 1'000'000>
struct fast_sieve_func_spf {
    vector<int> primes, spf;
    vector<ll> func;
    vector<char> is_prime;
    fast_sieve_func_spf() {
        is_prime.assign(n + 1, true);
        is_prime[0] = is_prime[1] = false;
        spf.resize(n + 1);

        func.resize(n + 1);
        func[1] = 1;

        for (int i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                primes.push_back(i), spf[i] = i;
                // handle the case of primes by updating func[i]
                func[i] = 1 + i;
            }
            int spfi = spf[i];
            for (const auto p : primes) {
                const int k = i * p;
                if (k > n) break;
                is_prime[k] = false;
                // p <= spfi
                spf[k] = p;
                if (spfi == p) {
                    // p divides i
                    // see how to update here
                    int w = i, pwi = p;
                    while (w % p == 0) pwi *= p, w /= p;
                    if (w == 1)
                        func[k] = 1 + p * func[i];
                    else
                        func[k] = func[w] * func[pwi];

                    break;
                } else {
                    // p and i are coprime
                    // see how to update here
                    func[k] = func[i] * func[p];
                }
            }
        }
    }
};

