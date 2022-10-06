#include <inttypes.h>

#define N_FACTORIAL 200000
#define MOD 998244353

typedef struct {
    int value;
} mint;

// don't use raw initialization for untrusted inputs
// a might be >= MOD
mint construct_mint(int a) {
    return (mint){a % MOD};
}

mint add(mint a, mint b) {
    mint ans = {a.value + b.value};
    if (ans.value >= MOD) ans.value -= MOD;
    return ans;
}

mint subtract(mint a, mint b) {
    mint ans = {a.value - b.value};
    if (ans.value < 0) ans.value += MOD;
    return ans;
}

mint multiply(mint a, mint b) {
    return (mint){INT64_C(1) * a.value * b.value % MOD};
}

mint power(mint a, int p) {
    mint ans = {1};
    while (p) {
        if (p & 1) ans = multiply(ans, a);
        if (p >>= 1) a = multiply(a, a);
    }
    return ans;
}

mint inverse(mint a) {
    assert(a.value != 0);
    return power(a, MOD - 2);
}

mint divide(mint a, mint b) {
    return multiply(a, inverse(b));
}

mint fact[N_FACTORIAL + 1], ifact[N_FACTORIAL + 1];

void precompute() {
    fact[0] = (mint){1};
    for (int i = 1; i <= N_FACTORIAL; ++i)
        fact[i] = multiply(fact[i - 1], (mint){i});
    ifact[N_FACTORIAL] = inverse(fact[N_FACTORIAL]);
    for (int i = N_FACTORIAL - 1; i >= 0; --i)
        ifact[i] = multiply(ifact[i + 1], (mint){i + 1});
}

mint C(int n, int r) {
    if (r < 0 || r > n) return (mint){0};
    return multiply(fact[n], multiply(ifact[r], ifact[n - r]));
}

