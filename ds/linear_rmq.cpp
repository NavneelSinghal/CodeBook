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
                const auto dfs = [&](const auto& self, int u) -> void {
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

    template <class T, class Combine>
    struct sparse_table {
        sparse_table(const std::vector<T>& a, const Combine& _combine)
            : combine(_combine) {
            build(a);
        }
        sparse_table(const Combine& _combine) : combine(_combine) {}
        void build(std::vector<T> a) {
            n = (int)a.size();
            s = std::move(a);
            if (n == 1) return;
            int log_n = std::__lg(n - 1) + 1;
            s.resize(n * log_n, s[0]);
            int current_offset = n;
            for (int j = 2; j < n; j <<= 1, current_offset += n) {
                int stride = 2 * j;
                for (int k = 0; k < n; k += stride) {
                    int middle = k + j;
                    int next_k = std::min(k + stride, n);
                    if (middle - 1 < n) {
                        s[current_offset + middle - 1] = s[middle - 1];
                        for (int l = middle - 2; l >= k; --l)
                            s[current_offset + l] =
                                combine(s[l], s[current_offset + l + 1]);
                    }
                    if (middle < n) {
                        s[current_offset + middle] = s[middle];
                        for (int l = middle + 1; l < next_k; ++l)
                            s[current_offset + l] =
                                combine(s[current_offset + l - 1], s[l]);
                    }
                }
            }
        }
        // [l, r)
        T query(int l, int r) {
            --r;
            if (l == r) return s[l];
            int base = std::__lg(l ^ r) * n;
            return combine(s[base + l], s[base + r]);
        }

       private:
        int n;
        std::vector<T> s;
        Combine combine;
    };

    template <class T>
    struct rmq {
        static constexpr int block_size = 8;
        static constexpr rmq_helper<int, block_size> r{};
        static constexpr auto combine = [](const T& a, const T& b) {
            return a < b ? a : b;
        };
        sparse_table<T, decltype(combine)> st{combine};
        rmq(const std::vector<T>& a) : full(a) {
            int n = (int)a.size();
            int compressed_size = (n + block_size - 1) / block_size;
            block_type.resize(compressed_size);
            n = compressed_size * block_size;
            full.resize(n, a[0]);
            debug(full);
            std::vector<T> compressed(compressed_size, a[0]);
            // if heavy, replace a[0] by something else
            for (int i = 0; i < n; i += block_size)
                compressed[i / block_size] = *min_element(
                    full.begin() + i, full.begin() + i + block_size);
            debug(compressed);
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
                    debug(bitset<64>(mask).to_string());
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

       private:
        std::vector<T> full;
        std::vector<int> block_type;
    };
}  // namespace RMQ

