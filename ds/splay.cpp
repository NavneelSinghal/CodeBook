template <class T>
struct SplayTree {
    struct Node {
        Node() {}
        Node(const T& _value) : value{_value} {}
        T value{};
        std::array<Node*, 2> child{};
        Node* parent{};
        // 0 -> left, 1 -> right
        bool side() const { return parent->child[1] == this; }
        // perform a rotation with the parent
        void rotate() {
            const auto p = parent;
            const bool i = side();
            if (p->parent)
                p->parent->attach(p->side(), this);
            else
                parent = nullptr;
            p->attach(i, child[!i]);
            attach(!i, p);
        }
        // splay this node
        void splay() {
            while (parent) {
                if (parent->parent)
                    (side() == parent->side() ? parent : this)->rotate();
                rotate();
            }
        }
        // split from the right child
        std::array<Node*, 2> split() {
            splay();
            const auto right = child[1];
            if (right) right->parent = nullptr;
            this->right = nullptr;
            return {this, right};
        }
        // attach a node on the specified side
        void attach(bool side, Node* const new_) {
            if (new_) new_->parent = this;
            child[side] = new_;
        }
    };
    struct iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = uint32_t;

       public:
        void operator--() { advance<false>(); }
        void operator++() { advance<true>(); }
        const T& operator*() { return node->value; }
        Node* node;
        iterator(Node* node_arg) : node(node_arg) {}
        bool operator==(const iterator oth) const { return node == oth.node; }
        bool operator!=(const iterator oth) const { return !(*this == oth); }

       private:
        template <bool dir>
        void advance() {
            if (node->child[1]) {
                node = extremum<!dir>(node->child[1]);
                return;
            }
            while (node->parent && node->side() == dir) node = node->parent;
            node = node->parent;
        }
    };
    Node* root{};
    uint32_t size_{};
    SplayTree() {}
    ~SplayTree() { destroy(root); }
    // destroy the subtree rooted at node
    static void destroy(Node* const node) {
        if (!node) return;
        for (Node* const child : node->child) destroy(child);
        delete node;
    }
    // insert a node
    void insert(Node* const x) {
        ++size_;
        if (!root) {
            root = x;
            return;
        }
        auto y = root;
        while (true) {
            auto& nw = y->child[x->value > y->value];
            if (!nw) {
                nw = x;
                nw->parent = y;
                root = nw;
                nw->splay();
                return;
            }
            y = nw;
        }
        return;
    }
    // insert a key
    void insert(const T& key) { insert(new Node{key}); }
    void erase(Node* const x) {
        assert(x);
        x->splay();
        root = join(x->child[0], x->child[1]);
        delete x;
        --size_;
    }
    void erase(const T& key) { erase(find(key)); }
    template <bool i>
    static Node* extremum(Node* x) {
        assert(x);
        while (x->child[i]) x = x->child[i];
        return x;
    }
    static Node* join(Node* const a, Node* const b) {
        if (!a) {
            b->parent = nullptr;
            return b;
        }
        Node* const mx = extremum<true>(a);
        mx->splay();
        assert(mx->child[1] == nullptr);
        mx->child[1] = b;
        mx->parent = nullptr;
        return mx;
    }
    Node* find(const T& key) {
        auto x = root;
        while (x && key != x->value) {
            const auto next = x->child[key > x->value];
            if (!next) x->splay();
            x = next;
        }
        return x;
    }
    size_t size() const { return size_; }
    bool empty() const { return size() == 0; }
    iterator begin() { return iterator{extremum<false>(root)}; }
    iterator end() { return iterator{nullptr}; }
};

