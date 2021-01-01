#include <cassert>
#include <iostream>

using namespace std;

int main() {
    int64_t n;
    cin >> n;
    int64_t bound = 1e8;
    // brute

    int64_t ans_brute = 0;
    if (n < bound) {
        for (int64_t i = 1; i <= n; ++i) {
            ans_brute += n / i;
        }
    }

    int64_t ans_clever = 0;
    // [i..last] is the interval satisfying n / x == n / i <=> x \in [i..last]
    for (int64_t i = 1, last; i <= n; i = last + 1) {
        last = n / (n / i);
        ans_clever += (last - i + 1) * (n / i);
    }

    if (n < bound) assert(ans_clever == ans_brute);
    cout << ans_clever << '\n';
    return 0;
}
