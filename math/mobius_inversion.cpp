// https://codeforces.com/blog/entry/53925

// g(n) = sum of f(d) where d | n
// f(n) = sum of g(d) mu(n / d) where d | n

// sum of mu(d) over d | n = (n == 1)

// can compute mu values using linear sieve

#include <iostream>
#include <vector>
using namespace std;

template <int N = 1'000'000>
struct MU {
    vector<int> primes, mu;
    vector<char> is_prime;
    MU() {
        is_prime.assign(N + 1, true);
        is_prime[0] = is_prime[1] = false;
        mu.resize(N + 1);
        mu[1] = 1;
        for (int i = 2; i <= N; ++i) {
            if (is_prime[i]) mu[i] = -1, primes.push_back(i);
            for (int j = 0; j < primes.size() && i * primes[j] <= N; ++j) {
                is_prime[i * primes[j]] = false;
                if (i % primes[j] == 0) {
                    mu[i * primes[j]] = 0;
                    break;
                } else {
                    mu[i * primes[j]] = -mu[i];
                }
            }
        }
    }
};

template <int N = 1'000'000>
struct PHI {
    vector<int> primes, phi;
    vector<char> is_prime;
    PHI() {
        is_prime.assign(N + 1, true);
        is_prime[0] = is_prime[1] = false;
        phi.resize(N + 1);
        phi[1] = 1;
        for (int i = 2; i <= N; ++i) {
            if (is_prime[i]) phi[i] = i - 1, primes.push_back(i);
            for (int j = 0; j < primes.size() && i * primes[j] <= N; ++j) {
                is_prime[i * primes[j]] = false;
                if (i % primes[j] == 0) {
                    phi[i * primes[j]] = primes[j] * phi[i];
                    break;
                } else {
                    phi[i * primes[j]] = (primes[j] - 1) * phi[i];
                }
            }
        }
    }
};

int main() {

    int n;
    cin >> n;
    int ans = 0;
   
    // finding the number of pairs (i, j) \in [n]^2 such that gcd(i, j) = 1
    /*MU<> s;
    for (int i = 1; i <= n; ++i) {
        ans += (n / i) * (n / i) * s.mu[i];
    }*/
    
    
    /*PHI<> s;
    for (int i = 1; i <= n; ++i) {
        ans += s.phi[i];
    }
    ans = ans * 2 - 1;
    */
    
    cout << ans << '\n';

}
