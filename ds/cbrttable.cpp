struct cube_root_table {
   private:
    int n, block_size;
    std::vector<std::vector<T>> brute, block_aggregate;
    std::vector<std::array<int, 2>> block_ends;

   public:
    // O(n^{4/3})
    cube_root_table(std::vector<T> a) {
        n = (int)a.size();
        block_size = std::cbrt(2 * n);
        int num_blocks = (n + block_size - 1) / block_size;
        brute.resize(n);
        block_aggregate.resize(num_blocks);
        block_ends.resize(num_blocks);
        for (int i = 0; i < num_blocks; ++i)
            block_ends[i] = {i * block_size,
                             std::min((i + 1) * block_size, n) - 1};
        for (auto [l, r] : block_ends) {
            for (int j = l; j <= r; ++j) {
                T start = a[j];
                brute[j].reserve(r - l + 1);
                brute[j].emplace_back(start);
                for (int k = j + 1; k <= r; ++k)
                    brute[j].emplace_back(combine(brute[j].back(), a[k]));
            }
        }
        for (int i = 0; i < num_blocks; ++i) {
            auto [l, r] = block_ends[i];
            block_aggregate[i].reserve(num_blocks - i);
            block_aggregate[i].emplace_back(brute[l][r - l]);
            for (int j = i + 1; j < num_blocks; ++j) {
                auto [l, r] = block_ends[j];
                block_aggregate[i].emplace_back(
                    combine(block_aggregate[i].back(), brute[l][r - l]));
            }
        }
    }

    // O(1)
    T query(int l, int r) const {
        assert(l <= r && r < n && l >= 0);
        int l_block = l / block_size;
        int r_block = r / block_size;
        if (l_block == r_block)
            return brute[l][r - l];
        else {
            int l_block_end = l_block * block_size + block_size - 1;
            int r_block_begin = r_block * block_size;
            if (l_block == r_block - 1)
                return combine(brute[l][l_block_end - l],
                               brute[r_block_begin][r - r_block_begin]);
            else
                return combine(
                    combine(
                        brute[l][l_block_end - l],
                        block_aggregate[l_block + 1][r_block - l_block - 2]),
                    brute[r_block_begin][r - r_block_begin]);
        }
    }
};


