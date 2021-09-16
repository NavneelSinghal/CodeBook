// min(x, y) returns the smaller index
template <class Min>
struct rmq {
    static constexpr int b = 31;
    static constexpr int all_mask = int((1U << b) - 1);
    int n;
    std::vector<int> mask;
    Min min;
    std::vector<std::vector<int>> jmp;
    int lsb(int x) { return x & -x; }
    int msb_index(int x) { return 31 ^ __builtin_clz(x); }
    int small(int r, int size = b) {
        int dist_from_r = msb_index(mask[r] & ((1 << size) - 1));
        return r - dist_from_r;
    }
    rmq(int _n, Min _min) : n(_n), mask(n), min(_min) {
        int curr_mask = 0;
        for (int i = 0; i < n; i++) {
            curr_mask = (curr_mask << 1) & all_mask;
            while (curr_mask > 0 and min(i, i - msb_index(lsb(curr_mask))) == i)
                curr_mask ^= lsb(curr_mask);
            curr_mask |= 1;
            mask[i] = curr_mask;
        }
        auto f = [this](int i) {
            return small(b * i + b - 1);
        };
        jmp = std::vector(1, std::vector<int>(n / b));
        int t = 0;
        std::generate(jmp[0].begin(), jmp[0].end(),
                      [&f, &t] { return f(t++); });
        for (int pw = 1, k = 1; pw * 2 <= n / b; pw *= 2, ++k) {
            jmp.emplace_back(n / b - pw * 2 + 1);
            for (int j = 0; j < (int)jmp[k].size(); ++j)
                jmp[k][j] = min(jmp[k - 1][j], jmp[k - 1][j + pw]);
        }
    }
    int query(int l, int r) {
        --r;
        if (r - l + 1 <= b) return small(r, r - l + 1);
        int ans = min(small(l + b - 1), small(r));
        int x = l / b + 1, y = r / b;
        if (x < y) {
            int dep = msb_index(y - x);
            ans = min(ans, min(jmp[dep][x], jmp[dep][y - (1 << dep)]));
        }
        return ans;
    }
};

namespace RMQ {
    template <class T, int N>
    struct simple_stack {
        T payload[N]{};
        int pos = 0;
        constexpr int size() const { return pos; }
        constexpr bool empty() const { return pos == 0; }
        constexpr void push(const T& t) { payload[pos++] = t; }
        constexpr T& top() { return payload[pos - 1]; }
        constexpr void clear() { pos = 0; }
        constexpr void pop() { --pos; }
    };

    // take block_size = 1/4 log_2 n
    // won't ever need more than 8
    // using 7 might be a bit better for memory, but constexpr is fine
    template <class T, int block_size = 8>
    struct rmq_helper {
        static constexpr int size = 1 << (2 * block_size);
        int min_index[size][block_size][block_size];
        constexpr rmq_helper() : min_index{} {
            // rmq() {
            simple_stack<int, block_size> stk;
            int left[block_size]{}, right[block_size]{};
            int order[block_size]{};
            for (int i = 0; i < size; ++i) {
                // compute the range mins for each of these separately
                if (__builtin_popcount(i) != block_size) continue;
                for (int j = 0; j < block_size; ++j) {
                    left[j] = -1;
                    right[j] = -1;
                }
                stk.clear();
                bool found = true;
                int cur = 0;
                int prev_popped = -1;
                for (int j = 0; j < 2 * block_size; ++j) {
                    int bit = (i >> j) & 1;
                    if (bit == 0) {
                        if (stk.empty()) {
                            found = false;
                            break;
                        }
                        prev_popped = stk.top();
                        stk.pop();
                    } else {
                        left[cur] = prev_popped;
                        if (!stk.empty()) right[stk.top()] = cur;
                        stk.push(cur);
                        cur++;
                        prev_popped = -1;
                    }
                }
                if (!found) continue;  // not a majorizable sequence
                int id = 0;
                const auto dfs = [&order, &id, &left, &right](const auto& self,
                                                              int u) -> void {
                    order[u] = id++;
                    if (left[u] != -1) self(self, left[u]);
                    if (right[u] != -1) self(self, right[u]);
                };
                while (!stk.empty()) {
                    prev_popped = stk.top();
                    stk.pop();
                }
                dfs(dfs, prev_popped);
                // now order[i] = preorder number which suffices
                auto better = [&order](int x, int y) {
                    return order[x] < order[y] ? x : y;
                };
                for (int j = 0; j < block_size; ++j) {
                    auto& mij = min_index[i][j];
                    mij[j] = j;
                    int x = j;
                    for (int k = j + 1; k < block_size; ++k) {
                        x = better(x, k);
                        mij[k] = x;
                    }
                }
            }
        }
    };

    template <class T>
    struct sparse_table {
        std::vector<std::vector<T>> jmp;
        void build (const std::vector<T>& V) {
            jmp = std::vector(1, std::vector<T>(V));
            for (int pw = 1, k = 1; pw * 2 <= (int)V.size(); pw *= 2, ++k) {
                jmp.emplace_back(V.size() - pw * 2 + 1);
                for (int j = 0; j < (int)jmp[k].size(); ++j)
                    jmp[k][j] = std::min(jmp[k - 1][j], jmp[k - 1][j + pw]);
            }
        }
        T query(int a, int b) {
            int dep = 31 - __builtin_clz(b - a);
            return std::min(jmp[dep][a], jmp[dep][b - (1 << dep)]);
        }
    };

    template <class T>
    struct rmq {
        // 8 works too
        static constexpr int block_size = 6;
        static constexpr rmq_helper<int, block_size> r{};
        static constexpr auto combine = [](const T& a, const T& b) {
            return a < b ? a : b;
        };
        sparse_table<T> st;
        rmq(int n) {
            full.resize(((n + block_size - 1) / block_size) * block_size);
        }
        void build() {
            int n = (int)full.size();
            int compressed_size = (n + block_size - 1) / block_size;
            block_type.resize(compressed_size);
            n = compressed_size * block_size;
            std::vector<T> compressed(compressed_size);
            // if heavy, replace a[0] by something else
            for (int i = 0; i < n; i += block_size)
                compressed[i / block_size] = *min_element(
                    full.begin() + i, full.begin() + i + block_size);
            st.build(compressed);
            simple_stack<int, block_size> stk;
            for (int i = 0; i < n; i += block_size) {
                stk.clear();
                int p = 1;
                int mask = 0;
                int last = i + block_size;
                for (int j = i; j < last; ++j) {
                    while (!stk.empty() && full[stk.top()] > full[j]) {
                        stk.pop();
                        p <<= 1;
                    }
                    stk.push(j);
                    mask ^= p;
                    p <<= 1;
                }
                block_type[i / block_size] = mask;
            }
        }

        // [i, j)
        T query(int i, int j) {
            --j;
            const int block_id_i = i / block_size;
            const int block_id_j = j / block_size;
            const int block_begin_i = block_id_i * block_size;
            const int block_end_i = block_id_i * block_size + block_size - 1;
            const int block_begin_j = block_id_j * block_size;
            const int block_type_i = block_type[block_id_i];
            const int block_type_j = block_type[block_id_j];
            if (block_id_i < block_id_j - 1) {
                T middle = st.query(block_id_i + 1, block_id_j);
                middle = std::min(
                    middle, full[r.min_index[block_type_i][i - block_begin_i]
                                            [block_end_i - block_begin_i] +
                                 block_begin_i]);
                return std::min(
                    middle,
                    full[r.min_index[block_type_j][0][j - block_begin_j] +
                         block_begin_j]);
            } else if (block_id_i == block_id_j - 1) {
                return std::min(
                    full[r.min_index[block_type_i][i - block_begin_i]
                                    [block_end_i - block_begin_i] +
                         block_begin_i],
                    full[r.min_index[block_type_j][0][j - block_begin_j] +
                         block_begin_j]);
            } else {
                return full[r.min_index[block_type_i][i - block_begin_i]
                                       [j - block_begin_i] +
                            block_begin_i];
            }
        }

        std::vector<T> full;
        std::vector<int> block_type;
    };
}  // namespace RMQ

