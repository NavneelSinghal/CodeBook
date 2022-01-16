// https://codeforces.com/blog/entry/98167

auto hook_length = [](int n, const vector<int>& partition) -> mint {
    // partition is sorted in non-increasing order
    vector<int> partition_transpose(partition[0]);
    for (int i = 0; i < partition[0]; ++i) {
        int x = 0;
        while (x < (int)partition.size() && partition[x] >= i + 1) ++x;
        partition_transpose[i] = x;
    }
    mint ans = 1;
    mint fact = 1;
    for (int i = 1; i <= n; ++i) fact *= i;
    for (int i = 0; i < (int)partition.size(); ++i)
        for (int j = 0; j < partition[i]; ++j)
            ans *= partition[i] + partition_transpose[j] - i - j - 1;
    return fact / ans;
};

