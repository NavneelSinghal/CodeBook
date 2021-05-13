struct cube_root_table {
   private:
    const int n, block_size, num_blocks;
    std::vector<T> brute, block_aggregate;
    std::vector<std::array<int, 2>> block_ends;

   public:
    // O(n^{4/3})
    cube_root_table(std::vector<T> a)
        : n((int)a.size()),
          block_size(std::cbrt(2 * a.size())),
          num_blocks((n + block_size - 1) / block_size) {
        brute.resize(num_blocks * block_size * (block_size + 1) / 2);
        block_aggregate.resize(num_blocks * (num_blocks + 1) / 2);
        block_ends.resize(num_blocks);
        for (int i = 0; i < num_blocks; ++i)
            block_ends[i] = {i * block_size,
                             std::min((i + 1) * block_size, n) - 1};
        for (auto [l, r] : block_ends) {
            for (int j = l; j <= r; ++j) {
                int loc = l * (block_size + 1) / 2 +
                          (j - l) * (2 * block_size - j + l + 1) / 2;
                brute[loc] = a[j];
                for (int k = j + 1; k <= r; ++k, ++loc)
                    brute[loc + 1] = combine(brute[loc], a[k]);
            }
        }
        for (int i = 0; i < num_blocks; ++i) {
            auto [l, r] = block_ends[i];
            int loc = i * num_blocks - i * (i - 1) / 2;
            block_aggregate[loc] = brute[l * (block_size + 1) / 2 + r - l];
            for (int j = i + 1; j < num_blocks; ++j, ++loc) {
                auto [l, r] = block_ends[j];
                block_aggregate[loc + 1] =
                    combine(block_aggregate[loc],
                            brute[l * (block_size + 1) / 2 + r - l]);
            }
        }
    }

    // O(1)
    T query(int l, int r) const {
        assert(l <= r && r < n && l >= 0);
        int l_block = l / block_size;
        int r_block = r / block_size;
        int l_block_begin = l_block * block_size;
        if (l_block == r_block)
            return brute[l_block_begin * (block_size + 1) / 2 +
                         (l - l_block_begin) *
                             (2 * block_size - l + l_block_begin + 1) / 2 +
                         r - l];
        else {
            int l_block_end = l_block * block_size + block_size - 1;
            int r_block_begin = r_block * block_size;
            auto brute_left =
                brute[l_block_begin * (block_size + 1) / 2 +
                      (l - l_block_begin) *
                          (2 * block_size - l + l_block_begin + 1) / 2 +
                      l_block_end - l];
            auto brute_right =
                brute[r_block_begin * (block_size + 1) / 2 + r - r_block_begin];
            if (l_block == r_block - 1)
                return combine(brute_left, brute_right);
            else {
                int w = (l_block + 1) * (2 * num_blocks - l_block) / 2;
                return combine(
                    brute_left,
                    combine(block_aggregate[w + r_block - l_block - 2],
                            brute_right));
            }
        }
    }
};

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
