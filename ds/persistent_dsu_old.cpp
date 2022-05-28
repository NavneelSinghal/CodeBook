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

template <int LogWidth>
struct PersistentDSU {
    PersistentArray<int, LogWidth> data;
    PersistentDSU() : data{} {}
    PersistentDSU(int sz) : data{} { data.build(std::vector<int>(sz, -1)); }
    int find(int k) {
        int p = data.get(k);
        return p >= 0 ? find(p) : k;
    }
    int size(int k) { return -data.get(find(k)); }
    bool join(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        auto u = data.get(x);
        auto v = data.get(y);
        if (u < v) {
            auto a = data.mutable_get(x);
            *a += v;
            auto b = data.mutable_get(y);
            *b = x;
        } else {
            auto a = data.mutable_get(y);
            *a += u;
            auto b = data.mutable_get(x);
            *b = y;
        }
        return true;
    }
};


