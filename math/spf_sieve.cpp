// memory - N + N/(log N)
template <int N = 1'000'000>
struct fast_sieve_spf {
    vector<int> primes, spf;
    fast_sieve_spf() {
        spf.resize(N + 1);
        for (int i = 2; i <= N; ++i) {
            if (spf[i] == 0) spf[i] = i, primes.push_back(i);
            int product, spfi = spf[i];
            for (auto prime : primes) {
                product = i * prime;
                if (product > N) break;
                spf[product] = prime;
                if (spfi == prime) break;
            }
        }
    }
};

