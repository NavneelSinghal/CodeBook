template <int n = 1'000'000>
struct fast_sieve_sumdiv {
    vector<int> primes, spf;
    vector<ll> func;
    vector<int> pw;
    vector<char> is_prime;
    fast_sieve_sumdiv() {
        is_prime.assign(n + 1, true);
        is_prime[0] = is_prime[1] = false;
        spf.resize(n + 1);

        func.resize(n + 1);
        func[1] = 1;
        pw.resize(n + 1);
        pw[1] = 1;

        for (int i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                primes.push_back(i), spf[i] = i;
                func[i] = 1 + i;
                pw[i] = i;
            }
            int spfi = spf[i];
            for (const auto p : primes) {
                const int k = i * p;
                if (k > n) break;
                is_prime[k] = false;
                spf[k] = p;
                if (spfi == p) {
                    int t = pw[k] = pw[i] * p;
                    if (k == t) func[k] = func[i] * p + 1;
                    else func[k] = func[k / t] * func[t];
                    break;
                } else {
                    func[k] = func[i] * func[p];
                    pw[k] = p;
                }
            }
        }
    }
};

