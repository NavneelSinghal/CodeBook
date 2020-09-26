typedef int T; // use mint instead
const int N = 1e6;

vector<int> fact(N + 1);
vector<int> ifact(N + 1);
vector<int> inv(N + 1);

void precompute_facts() {
    fact[0] = 1;
    for (int i = 1; i <= N; ++i) {
        fact[i] = fact[i - 1] * i;
    }
    ifact[N] = mod_inv(fact[N]);
    for (int i = N - 1; i >= 0; --i) {
        ifact[i] = ifact[i + 1] * (i + 1);
    }
    // i^(-1) = (i - 1)! / i!
    for (int i = 1; i <= N; ++i) {
        inv[i] = fact[i - 1] * ifact[i];
    }
}
