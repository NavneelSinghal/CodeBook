template <typename Key = uint32_t, int KeyLog = 30,
          int bitlen = sizeof(Key) * 8>
struct binary_trie {
   public:
    binary_trie() {}
    void insert(Key x, int k = 1) {
        int v = root;
        int bit = KeyLog - 1;
        while (bit >= 0) {
            nodes[v].count += k;
            int nxt = (x >> bit) & 1;
            if (nodes[v].ch[nxt] == -1) {
                nodes[v].ch[nxt] = make_node(x, 0, bit);
                nodes[nodes[v].ch[nxt]].count = k;
                return;
            }
            v = nodes[v].ch[nxt];
            int diff_bit = std::max(nodes[v].l - 1, next_bit(v, x));
            if (diff_bit == nodes[v].l - 1) {
                bit = diff_bit;
            } else {
                nxt = (x >> diff_bit) & 1;
                int tmp = make_node(nodes[v].k, nodes[v].l, diff_bit);
                nodes[v].l = diff_bit + 1;
                nodes[tmp].count = nodes[v].count;
                nodes[v].count += k;
                nodes[tmp].ch[!nxt] = nodes[v].ch[!nxt];
                nodes[tmp].ch[nxt] = nodes[v].ch[nxt];
                nodes[v].ch[!nxt] = tmp;
                nodes[v].ch[nxt] = make_node(x, 0, diff_bit);
                nodes[nodes[v].ch[nxt]].count = k;
                return;
            }
        }
        nodes[v].count += k;
    }
    int erase(Key x, int k = std::numeric_limits<int>::max()) {
        k = erase(x, k, root, KeyLog - 1);
        nodes[root].count -= k;
        return k;
    }
    int find(Key x) { return find(x, root, KeyLog - 1); }
    int size() { return nodes[root].count; }
    Key xor_min(Key x) {
        assert(size());
        int v = root;
        int bit = KeyLog - 1;
        Key k = 0;
        while (bit >= 0) {
            int target = (x >> bit) & 1;
            if (count(nodes[v].ch[target]) == 0) target ^= 1;
            v = nodes[v].ch[target];
            k += (x ^ nodes[v].k) & mask(nodes[v].l, nodes[v].r);
            bit = nodes[v].l - 1;
        }
        return k;
    }
    Key xor_max(Key x) {
        assert(size());
        int v = root;
        int bit = KeyLog - 1;
        Key k = 0;
        while (bit >= 0) {
            int target = ((x >> bit) & 1) ^ 1;
            if (count(nodes[v].ch[target]) == 0) target ^= 1;
            v = nodes[v].ch[target];
            k += (x ^ nodes[v].k) & mask(nodes[v].l, nodes[v].r);
            bit = nodes[v].l - 1;
        }
        return k;
    }
    // is there w in the trie such that l <= x ^ w < r
    bool xor_range_satisfiable(Key x, Key l, Key r) {
        if (l >= r) return false;
        r--;
        int v = root;
        for (int i = KeyLog - 1; i >= 0; i--) {
            if (((l >> i) & 1) == ((r >> i) & 1)) {
                int target = ((x >> i) & 1) ^ ((l >> i) & 1);
                v = next(v, i, target);
                if (count(v) == 0) return false;
            } else {
                int ith = (x >> i) & 1;
                int u = next(v, i, ith);
                if (u != -1) {
                    int j = i - 1;
                    for (; j >= 0 && u != -1; j--) {
                        int target = (l >> j) & 1, flip = (x >> j) & 1;
                        int ch_sz = count(next(u, j, flip ^ 1));
                        if (target == 1 && ch_sz == 0) break;
                        if (target == 0 && ch_sz) return true;
                        u = next(u, j, target ^ flip);
                    }
                    if (j == -1 && count(u)) return true;
                }
                u = next(v, i, ith ^ 1);
                if (u != -1) {
                    int j = i - 1;
                    for (; j >= 0 && u != -1; j--) {
                        int target = (r >> j) & 1, flip = (x >> j) & 1;
                        int ch_sz = count(next(u, j, flip));
                        if (target == 0 && ch_sz == 0) break;
                        if (target == 1 && ch_sz) return true;
                        u = next(u, j, target ^ flip);
                    }
                    if (j == -1 && count(u)) return true;
                }
                return false;
            }
        }
        return true;
    }

   private:
    struct node {
        Key k;
        int l, r, count;
        int ch[2];
        node(Key _k = 0, int _l = 0, int _r = 0)
            : k(_k), l(_l), r(_r), count(0) {
            ch[0] = ch[1] = -1;
        }
    };
    int root = 0, sz = 0;
    std::vector<node> nodes{node()};
    static Key mask(int l, int r) { return ((1LL << (r - l + 1)) - 1) << l; }
    int make_node(Key x, int l, int r) {
        nodes.push_back(node(x, l, r));
        return ++sz;
    }
    int next_bit(int v, Key k) {
        Key x = (nodes[v].k ^ k) & mask(nodes[v].l, nodes[v].r);
        if constexpr (bitlen > 32)
            return (x ? bitlen - 1 - __builtin_clzll(x) : -1);
        return (x ? bitlen - 1 - __builtin_clz(x) : -1);
    }
    int find(Key x, int v, int bit) {
        v = nodes[v].ch[(x >> bit) & 1];
        if (!v) return 0;
        int diff_bit = std::max(nodes[v].l - 1, next_bit(v, x));
        if (diff_bit != nodes[v].l - 1) return 0;
        if (nodes[v].l == 0) return nodes[v].count;
        return find(x, v, diff_bit);
    }
    int count(int v) { return (v == -1 ? 0 : nodes[v].count); }
    int erase(Key x, int k, int v, int bit) {
        v = nodes[v].ch[(x >> bit) & 1];
        if (count(v) == 0) return 0;
        int diff_bit = std::max(nodes[v].l - 1, next_bit(v, x));
        if (diff_bit > nodes[v].l - 1) return 0;
        if (nodes[v].l == 0) {
            k = std::min(k, nodes[v].count);
            nodes[v].count -= k;
            return k;
        }
        k = erase(x, k, v, diff_bit);
        nodes[v].count -= k;
        return k;
    }
    int next(int v, int k, bool is_right) {
        if (v == root || k == nodes[v].l - 1) {
            return nodes[v].ch[is_right];
        } else {
            if (((nodes[v].k >> k) & 1) == is_right)
                return v;
            else
                return -1;
        }
    }
};

