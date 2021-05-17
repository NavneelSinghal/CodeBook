#pragma GCC optimize("Ofast")

#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

/*

contents:

1. sieves - linear and n log log n
2. spf sieves - linear and n log log n
3. benq implementation
4. finding all primes in l..r with r - l ~ 1e5 and r ~ 1e12
5. compute a multiplicative function in O(N)

results for the simulation:

1e8:
Ofast

Time taken by fast (linear) sieve : 1.17568s
Time taken by N log log N sieve : 1.18992s
Time taken by Benq's sieve : 2.29913s
Time taken by linear spf sieve : 1.15256s
Time taken by N log log N spf sieve : 1.53179s

*/

// memory - N + N/(log N)
template <int N = 1'000'000>
struct sieve {
    vector<char> is_prime;
    vector<int> primes;
    sieve() {
        is_prime.assign(N + 1, true);
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i * i <= N; ++i)
            if (is_prime[i])
                for (int j = i * i; j <= N; j += i) is_prime[j] = false;
        for (int i = 2; i <= N; ++i)
            if (is_prime[i]) primes.push_back(i);
    }
};

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

// memory - N + N/(log N)
template <int N = 1'000'000>
struct spf_sieve {
    // vector<char> is_prime;
    vector<int> spf, primes;
    // is_prime[i] = (spf[i] == i) && i
    spf_sieve() {
        // is_prime.assign(N + 1, true);
        spf.resize(N + 1);
        // is_prime[0] = is_prime[1] = false;
        for (int i = 2; i <= N; ++i)
            // if (is_prime[i])
            if (!spf[i]) {
                spf[i] = i, primes.push_back(i);
                if ((long long)i * i > N) continue;
                for (int j = i * i; j <= N; j += i) {
                    // is_prime[j] = false;
                    if (!spf[j]) spf[j] = i;
                }
            }
    }
};

// memory - N + N/(log N)
template <int N = 1'000'000>
struct fast_sieve {
    vector<int> primes;
    vector<char> is_prime;
    fast_sieve() {
        is_prime.assign(N + 1, true);
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i <= N; ++i) {
            if (is_prime[i]) primes.push_back(i);
            for (int j = 0; j < primes.size() && i * primes[j] <= N; ++j) {
                is_prime[i * primes[j]] = false;
                if (i % primes[j] == 0) break;
            }
        }
    }
};

// memory - N + N/(log N)
template <int N = 1'000'000>
struct fast_sieve_spf {
    vector<int> primes, spf;
    // vector<char> is_prime;
    fast_sieve_spf() {
        // is_prime.assign(N + 1, true);
        spf.resize(N + 1);
        for (int i = 2; i <= N; ++i) {
            if (spf[i] == 0) spf[i] = i, primes.push_back(i);
            int product, spfi = spf[i];
            for (int j = 0; j < primes.size() && i * primes[j] <= N; ++j) {
                spf[i * primes[j]] = primes[j];
                if (spfi == primes[j]) break;
            }
            /*for (auto &prime : primes) {
                product = i * prime;
                if (product > N) break;
                spf[product] = prime;
                // is_prime[product] = false;
                if (spfi == prime) break;
            }*/
        }
    }
};

// memory - N + N/(log N)
template <int SZ>
struct Sieve {
    vector<int> pr;
    /*
    bitset<SZ + 1> pri;
    Sieve() {
        pri.set(); pri[0] = pri[1] = 0;
        for (int i = 4; i <= SZ; i += 2) pri[i] = 0;
        for (int i = 3; i * i <= SZ; i += 2) if (pri[i]) for (int j = i * i; j
    <= SZ; j += i << 1) pri[j] = 0; for (int i = 1; i <= SZ; ++i) if (pri[i])
    pr.push_back(i);
        }
    */

    vector<int> spf;

    Sieve() {  // above is faster
        spf.resize(SZ + 1);
        for (int i = 2; i <= SZ; ++i) {
            if (spf[i] == 0) spf[i] = i, pr.push_back(i);
            for (auto p : pr) {
                if (p > spf[i] || i * p > SZ) break;
                spf[i * p] = p;
            }
        }
    }
};

// find all primes between L and R inclusive, when R ~ 1e12
// complexity: (R - L + 1) log log R + sqrt(R) log log sqrt(R)
vector<bool> segmentedSieve(long long L, long long R) {
    // generate all primes up to sqrt(R)
    long long lim = sqrt(R);
    vector<bool> mark(lim + 1, false);
    vector<long long> primes;
    for (long long i = 2; i <= lim; ++i) {
        if (!mark[i]) {
            primes.emplace_back(i);
            for (long long j = i * i; j <= lim; j += i) mark[j] = true;
        }
    }

    vector<bool> isPrime(R - L + 1, true);
    for (long long i : primes)
        for (long long j = max(i * i, (L + i - 1) / i * i); j <= R; j += i)
            isPrime[j - L] = false;
    if (L == 1) isPrime[0] = false;
    return isPrime;
}

// complexity: (R - L + 1) log R + sqrt(R)
vector<bool> segmentedSieveNoPreGen(long long L, long long R) {
    vector<bool> isPrime(R - L + 1, true);
    long long lim = sqrt(R);
    for (long long i = 2; i <= lim; ++i)
        for (long long j = max(i * i, (L + i - 1) / i * i); j <= R; j += i)
            isPrime[j - L] = false;
    if (L == 1) isPrime[0] = false;
    return isPrime;
}

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
            if (is_prime[i]) primes.push_back(i);
            // here you should handle the case of primes by updating func[i]
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

// memory - 3n + n/(log n)
// how to compute a multiplicative function in o(n) for all values
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



// memory - n + n/(log n)
// how to compute a multiplicative function in o(n) for all values
template <int n = 1'000'000>
struct fast_sieve_func {
    vector<int> primes, func;
    vector<char> is_prime;
    fast_sieve_func() {
        is_prime.assign(n + 1, true);
        is_prime[0] = is_prime[1] = false;
        func.resize(n + 1);
        func[1] = 1;  // for multiplicative functions, it is either 0 or 1
        for (int i = 2; i <= n; ++i) {
            if (is_prime[i]) primes.push_back(i);
            // and handle the case of primes by updating func[i]
            for (int j = 0; j < primes.size() && i * primes[j] <= n; ++j) {
                is_prime[i * primes[j]] = false;
                // update func[i * primes[j]] in the following branches
                if (i % primes[j] == 0) {
                    // primes[j] divides i
                    // see how to update here
                    break;
                } else {
                    // primes[j] and i are coprime
                    // see how to update here
                }
            }
        }
    }
};

int main() {
    const int N = 1e8;

    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

    //#if 0
    start = std::chrono::high_resolution_clock::now();
    fast_sieve<N> s1;
    stop = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    cerr << "Time taken by fast (linear) sieve : "
         << ((long double)duration.count()) / ((long double)1e9) << "s "
         << endl;

    start = std::chrono::high_resolution_clock::now();
    sieve<N> s2;
    stop = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    cerr << "Time taken by N log log N sieve : "
         << ((long double)duration.count()) / ((long double)1e9) << "s "
         << endl;
    //#endif

    //#if 0
    start = std::chrono::high_resolution_clock::now();
    Sieve<N> s4;
    stop = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    cerr << "Time taken by Benq's sieve : "
         << ((long double)duration.count()) / ((long double)1e9) << "s "
         << endl;
    //#endif

    //#if 0
    start = std::chrono::high_resolution_clock::now();
    fast_sieve_spf<N> s3;
    stop = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    cerr << "Time taken by linear spf sieve : "
         << ((long double)duration.count()) / ((long double)1e9) << "s "
         << endl;

    start = std::chrono::high_resolution_clock::now();
    spf_sieve<N> s5;
    stop = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    cerr << "Time taken by N log log N spf sieve : "
         << ((long double)duration.count()) / ((long double)1e9) << "s "
         << endl;
    //#endif

    for (int i = 0; i <= N; ++i) {
        assert(s5.spf[i] == s3.spf[i]);
        assert(s5.spf[i] == s4.spf[i]);
    }

    assert(s1.primes == s2.primes);
    assert(s2.primes == s3.primes);
    assert(s3.primes == s5.primes);
    assert(s5.primes == s4.pr);

    // assert(false);
}
