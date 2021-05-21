namespace subsetTransforms {
    // sum over subsets dp - for every index i, it finds the sum of all f(j)
    // where j is a subset of i in binary
    template <typename T>
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

    // mobius dp - inclusion exclusion, inverse of zeta transform
    template <typename T>
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

