#include <bits/stdc++.h>
using namespace std;

const int mod = 1e9 + 7;

namespace subsetTransforms {
template <typename T>  // sum over subsets dp - for every index i, it finds the
                       // sum of all f(j) where j is a subset of i in binary
vector<T> zetaTransform(vector<T> &v) {
    int n = v.size();
    assert((n & (n - 1)) == 0);
    vector<T> result(v);
    for (int i = 1; i < n; i <<= 1) {
        for (int mask = 0; mask < n; ++mask) {
            if (mask & i) {
                result[mask] += result[mask ^ i];
                // to compute over supersets, just swap these two
            }
        }
    }
    return result;
}

template <typename T>  // mobius dp - inclusion exclusion, inverse of zeta
                       // transform
vector<T> mobiusTransform(vector<T> &v) {
    int n = v.size();
    assert((n & (n - 1)) == 0);
    vector<T> result(v);
    for (int i = 1; i < n; i <<= 1) {
        for (int mask = 0; mask < n; ++mask) {
            if (mask & i) {
                result[mask] -= result[mask ^ i];
                // to retrieve from superset sums instead, swap these two
            }
        }
    }
}

}  // namespace subsetTransforms

using namespace IO;

int main() {
    IOinit();
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int n;
    read_int(n);
    int N = 1 << n;

    vector<int> a(N), b(N), c(N);

    for (int i = 0; i < N; ++i) read_int(a[i]);
    for (int i = 0; i < N; ++i) read_int(b[i]);
    for (int i = 0; i < N; ++i) read_int(c[i]);

    for (int i = 1; i < N; i <<= 1) {
        for (int mask = 0; mask < N; ++mask) {
            if (mask & i) {
                int mm = mask ^ i;
                a[mask] += a[mm];
                if (a[mask] >= mod) a[mask] -= mod;
                b[mask] += b[mm];
                if (b[mask] >= mod) b[mask] -= mod;
                c[mask] += c[mm];
                if (c[mask] >= mod) c[mask] -= mod;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        a[i] = ((long long)a[i] * b[i]) % mod;
        a[i] = ((long long)a[i] * c[i]) % mod;
    }

    for (int i = 1; i < N; i <<= 1) {
        for (int mask = 0; mask < N; ++mask) {
            if (mask & i) {
                a[mask] -= a[mask ^ i];
                if (a[mask] < 0) a[mask] += mod;
            }
        }
    }

    for (int i = 1; i < N; i <<= 1) {
        for (int mask = 0; mask < N; ++mask) {
            if (mask & i) {
                int mm = mask ^ i;
                a[mm] += a[mask];
                if (a[mm] >= mod) a[mm] -= mod;
            }
        }
    }

    int sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += a[i];
        if (sum >= mod) sum -= mod;
    }

    write_int(sum, '\n');
    _flush_output();
}
