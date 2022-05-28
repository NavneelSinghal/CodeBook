// Typical LogWidth = 3
template <typename T, int LogWidth>
struct PersistentArray {
    static constexpr int Mask = (1 << LogWidth) - 1;
    struct WideNode {
        T data;
        std::array<WideNode*, 1 << LogWidth> child;
        WideNode() : data{}, child{} {}
        WideNode(const T& data) : data{data}, child{} {}
    };

    WideNode* root;
    inline static std::deque<WideNode> buffer;

    PersistentArray() : root{nullptr} {}

    // value at index k
    T get(int k) { return get(root, k); }

    // mutable value at index k
    T* mutable_get(int k) {
        auto [new_root, value_ptr] = mutable_get(root, k);
        root = new_root;
        return value_ptr;
    }

    // build from std::vector
    void build(const std::vector<T>& v) {
        root = nullptr;
        for (int i = 0; i < (int)v.size(); i++) root = build(root, v[i], i);
    }

   private:
    T get(WideNode* t, int k) {
        if (k == 0) return t->data;
        return get(t->child[k & Mask], k >> LogWidth);
    }
    std::pair<WideNode*, T*> mutable_get(WideNode* t, int k) {
        t = t ? &buffer.emplace_back(*t) : &buffer.emplace_back();
        if (k == 0) return {t, &t->data};
        auto [ret_root, ret_value_ptr] =
            mutable_get(t->child[k & Mask], k >> LogWidth);
        t->child[k & Mask] = ret_root;
        return {t, ret_value_ptr};
    }
    WideNode* build(WideNode* t, const T& data, int k) {
        if (!t) t = &buffer.emplace_back();
        if (k == 0) {
            t->data = data;
            return t;
        }
        auto p = build(t->child[k & Mask], data, k >> LogWidth);
        t->child[k & Mask] = p;
        return t;
    }
};

