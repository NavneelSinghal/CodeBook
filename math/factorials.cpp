const int N = 1e6;

vector<mint> fact(N + 1);
vector<mint> ifact(N + 1);
vector<mint> inv(N + 1);

void precompute_facts() {
    fact[0] = 1;
    for (int i = 1; i <= N; ++i) {
        fact[i] = fact[i - 1] * i;
    }
    ifact[N] = 1 / fact[N];
    for (int i = N - 1; i >= 0; --i) {
        ifact[i] = ifact[i + 1] * (i + 1);
    }
    // i^(-1) = (i - 1)! / i!
    for (int i = 1; i <= N; ++i) {
        inv[i] = fact[i - 1] * ifact[i];
    }
}

auto C = [&](int n, int r) -> mint {
    if (r < 0 || r > n) return 0;
    return fact[n] * ifact[n - r] * ifact[r];
};
