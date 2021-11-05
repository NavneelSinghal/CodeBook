template <class BlockSize, class Node, class AL, class AR, class EL, class ER>
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
    };
    static constexpr int B = BlockSize::value;

   public:
    Mo(int _n, BlockSize dummy,
       const std::vector<std::pair<int, int>>& _queries, const Node& _id_node,
       const AL& _add_left, const AR& _add_right, const EL& _erase_left,
       const ER& _erase_right)
        : ans(_queries.size()),
          id_node(_id_node),
          add_left(_add_left),
          add_right(_add_right),
          erase_left(_erase_left),
          erase_right(_erase_right) {
        int q = (int)_queries.size();
        int n = _n;
        int N = 2 * n - 1;
        int lgn = 0;
        while ((1 << lgn) <= N) ++lgn;
        --lgn;
        std::vector<int> id(q);
        std::iota(id.begin(), id.end(), 0);
        std::sort(id.begin(), id.end(), [&_queries](int i, int j) {
            return _queries[i].first / B != _queries[j].first / B
                       ? _queries[i] < _queries[j]
                   : _queries[i].first / B & 1
                       ? _queries[i].second < _queries[j].second
                       : _queries[i].second > _queries[j].second;
        });
        int l = 0, r = -1;
        Node cur = id_node;
        for (auto i : id) {
            auto [ql, qr] = _queries[i];
            while (l > ql) add_left(--l, cur);
            while (r < qr) add_right(++r, cur);
            while (l < ql) erase_left(l++, cur);
            while (r > qr) erase_right(r--, cur);
            ans[i] = cur;
        }
    }
    std::vector<Node> get() { return ans; }
};

struct BlockSize {
    static constexpr int value = 500;
} dummy;


