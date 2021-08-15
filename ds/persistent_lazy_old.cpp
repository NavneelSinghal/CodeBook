template <class Node, class Update, class CombineNodes, class ApplyUpdate,
          class ComposeUpdates = std::nullptr_t>
struct PersistentSegTree {
    static constexpr bool is_lazy =
        !std::is_same<ComposeUpdates, std::nullptr_t>::value;
    CombineNodes combine;
    Node id_node;
    Update id_update;
    ApplyUpdate apply_update;
    ComposeUpdates compose_updates;
    using ll = long long;
    struct STNode_false {
        Node node;
        STNode_false *l, *r;
        STNode_false() {}
        STNode_false(const Node& _node) : node(_node), l(nullptr), r(nullptr) {}
    };
    struct STNode_true {
        Node node;
        Update update;
        STNode_true *l, *r;
        STNode_true() {}
        STNode_true(const Node& _node, const Update& _update)
            : node(_node), update(_update), l(nullptr), r(nullptr) {}
    };
    using STNode =
        typename std::conditional<is_lazy, STNode_true, STNode_false>::type;
    std::deque<STNode> pool;
    int pid;
    ll N;
    STNode* nil;
    vector<STNode*> roots;

    explicit PersistentSegTree(const std::vector<Node>& v, const Node& _id_node,
                               const CombineNodes& _combine,
                               const Update& _id_update,
                               const ApplyUpdate& _apply_update,
                               const ComposeUpdates& _compose_updates = nullptr)
        : N(int(v.size())),
          pid(0),
          nil(nullptr),
          combine(_combine),
          id_node(_id_node),
          apply_update(_apply_update),
          id_update(_id_update),
          compose_updates(_compose_updates) {
        pool.resize(16);
        nil = my_new(id_node);
        nil->l = nil->r = nil;
        roots.push_back(build(v));
    }

    explicit PersistentSegTree(const ll& _N, const Node& _id_node,
                               const CombineNodes& _combine,
                               const Update& _id_update,
                               const ApplyUpdate& _apply_update,
                               const ComposeUpdates& _compose_updates = nullptr)
        : N(_N),
          pid(0),
          nil(nullptr),
          combine(_combine),
          id_node(_id_node),
          apply_update(_apply_update),
          id_update(_id_update),
          compose_updates(_compose_updates) {
        pool.resize(16);
        nil = my_new(id_node);
        nil->l = nil->r = nil;
        roots.push_back(nil);
    }

    inline void buffer_nodes() {
        if (pid + 1 >= pool.size()) pool.resize(pool.size() + 1000);
    }

    STNode* my_new(const Node& node) {
        pool[pid].node = node;
        if constexpr (is_lazy) pool[pid].update = id_update;
        pool[pid].l = pool[pid].r = nil;
        buffer_nodes();
        return &(pool[pid++]);
    }
    STNode* merge(STNode* l, STNode* r) {
        pool[pid].node = combine(l->node, r->node);
        if constexpr (is_lazy) pool[pid].update = id_update;
        pool[pid].l = l;
        pool[pid].r = r;
        buffer_nodes();
        return &(pool[pid++]);
    }
    STNode* build(const vector<Node>& v) {
        N = (ll)v.size();
        return build(0, (ll)v.size(), v);
    }
    STNode* build(ll l, ll r, const vector<Node>& v) {
        if (l + 1 == r) return my_new(v[l]);
        ll m = (l + r) >> 1;
        return merge(build(l, m, v), build(m, r, v));
    }

   private:
    STNode* new_lazy_child(STNode* n, const Update& u) {
        pool[pid].node = apply_update(u, n->node);
        pool[pid].l = n->l;
        pool[pid].r = n->r;
        if constexpr (is_lazy) pool[pid].update = compose_updates(u, n->update);
        buffer_nodes();
        return &(pool[pid++]);
    }
    void pushdown_(STNode* n) {
        if constexpr (is_lazy) {
            if (n->update == id_update) return;
            n->l = new_lazy_child(n->l, n->update);
            n->r = new_lazy_child(n->r, n->update);
            n->update = id_update;
        }
    }
    STNode* update_(ll a, const Update& x, STNode* n, ll l, ll r) {
        if (l + 1 == r) return my_new(apply_update(x, n->node));
        ll m = (l + r) >> 1;
        if constexpr (is_lazy) pushdown_(n);
        if (a < m) return merge(update_(a, x, n->l, l, m), n->r);
        return merge(n->l, update_(a, x, n->r, m, r));
    }
    Node query_(ll a, ll b, STNode* n, ll l, ll r) {
        if (n == nil or r <= a or b <= l)
            return id_node;  // nullptr never possible
        if (a <= l and r <= b) return n->node;
        ll m = (l + r) >> 1;
        if constexpr (is_lazy) pushdown_(n);
        return combine(query_(a, b, n->l, l, m), query_(a, b, n->r, m, r));
    }
    STNode* update_(ll L, ll R, const Update& x, STNode* n, ll l, ll r) {
        if (L <= l and r <= R) return my_new(apply_update(x, n->node));
        ll m = (l + r) >> 1;
        if constexpr (is_lazy) pushdown_(n);
        STNode* left = n->l;
        STNode* right = n->r;
        if (L < m) left = update_(L, R, x, n->l, l, m);
        if (m < R) right = update_(L, R, x, n->r, m, r);
        return merge(left, right);
    }

   public:
    STNode* update(STNode* n, ll k, const Update& x) {
        STNode* root = update_(k, x, n, 0, N);
        roots.push_back(root);
        return root;
    }
    STNode* update(int t, ll k, const Update& x) {
        STNode* root = update_(k, x, roots[t], 0, N);
        roots.push_back(root);
        return root;
    }
    STNode* update(STNode* n, ll a, ll b, const Update& x) {
        assert(0 <= a && a < b && b <= N);
        STNode* root = update_(a, b, x, n, 0, N);
        roots.push_back(root);
        return root;
    }
    STNode* update(int t, ll a, ll b, const Update& x) {
        assert(0 <= a && a < b && b <= N);
        STNode* root = update_(a, b, x, roots[t], 0, N);
        roots.push_back(root);
        return root;
    }
    STNode* update_last(ll k, const Update& x) {
        STNode* root = update_(k, x, roots.back(), 0, N);
        roots.push_back(root);
        return root;
    }
    STNode* update_last(ll a, ll b, const Update& x) {
        assert(0 <= a && a < b && b <= N);
        STNode* root = update_(a, b, x, roots.back(), 0, N);
        roots.push_back(root);
        return root;
    }

    Node query(STNode* n, ll a, ll b) {
        assert(0 <= a && a < b && b <= N);
        return query_(a, b, n, 0, N);
    }
    Node query(int t, ll a, ll b) {
        assert(0 <= a && a < b && b <= N);
        return query_(a, b, roots[t], 0, N);
    }
    Node query_last(ll a, ll b) {
        assert(0 <= a && a < b && b <= N);
        return query_(a, b, roots.back(), 0, N);
    }
    STNode* new_tree() { return nil; }
};

