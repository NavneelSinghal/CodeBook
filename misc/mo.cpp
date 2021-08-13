template <class Node, class AL, class AR, class EL, class ER>
struct Mo {
   private:
    std::vector<Node> ans;
    Node id_node;
    AL add_left;
    AR add_right;
    EL erase_left;
    ER erase_right;
    struct Query {
        int l, r;
        int64_t order;
        Query() {}
        inline int64_t hilbert_order(int x, int y, int p, int to_rotate) {
            if (p == 0) return 0;
            static constexpr int rotate_delta[4] = {3, 0, 0, 1};
            int hp = 1 << (p - 1);
            int seg = (x < hp) ? ((y < hp) ? 0 : 3) : ((y < hp) ? 1 : 2);
            seg = (seg + to_rotate) & 3;
            int nx = x & (x ^ hp), ny = y & (y ^ hp);
            int nrot = (to_rotate + rotate_delta[seg]) & 3;
            int64_t sub_square_size = int64_t(1) << (2 * p - 2);
            int64_t ans = seg * sub_square_size;
            int64_t add = hilbert_order(nx, ny, p - 1, nrot);
            ans += (seg == 1 || seg == 2) ? add : (sub_square_size - add - 1);
            return ans;
        }
        Query(int _l, int _r, int lgn) : l(_l), r(_r) {
            order = hilbert_order(_l, _r, lgn, 0);
        }
        bool operator<(const Query& q) const { return order < q.order; }
    };
    template <class T, bool unsafe = false>
    __attribute__((target("bmi,bmi2,popcnt,lzcnt"))) constexpr uint32_t clz(
        T a) {
        if constexpr (!unsafe) {
            if (!a) return sizeof(T) * 8;
        }
        if constexpr (sizeof(T) <= sizeof(uint32_t)) {
            return (uint32_t)(__builtin_clz((uint32_t)a));
        } else if constexpr (sizeof(T) <= sizeof(uint64_t)) {
            return (uint32_t)(__builtin_clzll((uint64_t)a));
        } else {
            static_assert(sizeof(T) == sizeof(uint64_t) * 2);
            uint32_t l = clz((uint64_t)(a >> sizeof(uint64_t) * 8));
            return l != sizeof(uint64_t) * 8 ? l : l + clz((uint64_t)a);
        }
    }
    template <class T, bool unsafe = false>
    __attribute__((target("bmi,bmi2,popcnt,lzcnt"))) constexpr uint32_t lg(
        T a) {
        if constexpr (sizeof(T) <= sizeof(uint32_t)) {
            return sizeof(uint32_t) * 8 - 1 - clz<T, unsafe>(a);
        } else if constexpr (sizeof(T) <= sizeof(uint64_t)) {
            return sizeof(uint64_t) * 8 - 1 - clz<T, unsafe>(a);
        } else {
            return sizeof(uint64_t) * 16 - 1 - clz<T, unsafe>(a);
        }
    }

   public:
    Mo(int _n, const std::vector<std::pair<int, int>>& _queries,
       const Node& _id_node, const AL& _add_left, const AR& _add_right,
       const EL& _erase_left, const ER& _erase_right)
        : ans(_queries.size()),
          id_node(_id_node),
          add_left(_add_left),
          add_right(_add_right),
          erase_left(_erase_left),
          erase_right(_erase_right) {
        int q = (int)_queries.size();
        int n = _n;
        std::vector<Query> queries;
        queries.reserve(q);
        int lgn = lg(2 * n - 1);
        for (int i = 0; i < q; ++i) {
            auto [l, r] = _queries[i];
            queries.push_back(Query(l, r, lgn));
        }
        std::vector<int> id(q);
        std::iota(id.begin(), id.end(), 0);
        std::sort(id.begin(), id.end(),
                  [&queries](int i, int j) { return queries[i] < queries[j]; });
        int l = 0, r = -1;
        Node cur = id_node;
        for (auto i : id) {
            auto [ql, qr, _] = queries[i];
            while (l > ql) add_left(--l, cur);
            while (r < qr) add_right(++r, cur);
            while (l < ql) erase_left(l++, cur);
            while (r > qr) erase_right(r--, cur);
            ans[i] = cur;
        }
    }
    std::vector<Node> get() { return ans; }
};

