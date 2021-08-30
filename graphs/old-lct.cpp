namespace link_cut {
    struct Empty {};
    template <typename Node>
    struct PathParent {
        Node* path_parent{};
    };
    template <typename T, bool has_link_cut = false>
    struct SplayNode
        : public std::conditional_t<
              has_link_cut, PathParent<SplayNode<T, has_link_cut>>, Empty> {
       public:
        SplayNode() {}
        SplayNode(T value_arg) : value{value_arg} {}
        T value{};                          //!< Value associated with node
        std::array<SplayNode*, 2> child{};  //!< Left and right children
        SplayNode* parent{};                //!< Pointer to parent
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
            if constexpr (has_link_cut) this->path_parent = p->path_parent;
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
        std::array<SplayNode*, 2> split() {
            splay();
            const auto right = child[1];
            if (right) right->parent = nullptr;
            this->right = nullptr;
            return {this, right};
        }
        // attach a node on the specified side
        void attach(bool side, SplayNode* const new_) {
            if (new_) new_->parent = this;
            child[side] = new_;
        }
    };
    template <typename T>
    struct SplayTree {
       public:
        using Node = SplayNode<T>;
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
            bool operator==(const iterator oth) const {
                return node == oth.node;
            }
            bool operator!=(const iterator oth) const {
                return !(*this == oth);
            }

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
        std::size_t size_{};
        SplayTree() {}
        ~SplayTree() { destroy(root); }
        // destroy the subtree rooted at node
        static void destroy(Node* const node) {
            if (!node) return;
            for (Node* const child : node->child) destroy(child);
            delete node;
        }
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
        void insert(const T& key) { insert(new SplayNode{key}); }
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
        std::size_t size() const { return size_; }
        bool empty() const { return size() == 0; }
        iterator begin() { return iterator{extremum<false>(root)}; }
        iterator end() { return iterator{nullptr}; }
    };

    using Node = SplayNode<std::int64_t, true>;
    Node* make_tree() { return new Node{}; }
    void detach_child(Node* node) {
        if (node->child[1]) {
            node->child[1]->path_parent = node;
            node->child[1]->parent = nullptr;
        }
    }
    Node* access(Node* node) {
        node->splay();
        detach_child(node);
        node->child[1] = nullptr;
        Node* par = node;
        while (node->path_parent) {
            par = node->path_parent;
            par->splay();
            detach_child(par);
            par->attach(1, node);
            node->path_parent = nullptr;
            node->splay();
        }
        return par;
    }
    Node* find_root(Node* node) {
        access(node);
        while (node->child[0]) node = node->child[0];
        access(node);
        return node;
    }
    void cut(Node* node) {
        access(node);
        node->child[0]->parent = nullptr;
        node->child[0] = nullptr;
    }
    void link(Node* par, Node* child) {
        access(child);
        access(par);
        child->attach(0, par);
    }
    Node* lca(Node* u, Node* v) {
        if (find_root(u) != find_root(v)) return nullptr;
        access(u);
        return access(v);
    }
}  // namespace link_cut
