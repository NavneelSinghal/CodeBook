// https://atcoder.jp/contests/atc002/submissions/30383964
// https://math.mit.edu/~shor/PAM/hu-tucker_algorithm.html
template <typename T, typename E, typename F, typename G, typename C, T INF,
          E e>
struct SkewHeap {
    F f;
    G g;
    C c;
    SkewHeap(F f, G g, C c) : f(f), g(g), c(c) {}

    struct Node {
        Node *l, *r;
        T val;
        E add;
        Node(T val, E add) : val(val), add(add) { l = r = nullptr; }
    };

    void eval(Node* a) {
        if (!a) return;
        if (a->add == e) return;
        if (a->l) a->l->add = g(a->l->add, a->add);
        if (a->r) a->r->add = g(a->r->add, a->add);
        a->val = f(a->val, a->add);
        a->add = e;
    }

    T top(Node* a) { return a ? f(a->val, a->add) : INF; }

    T worse_child(Node* a) {
        eval(a);
        return a ? std::min(top(a->l), top(a->r), c) : INF;
    }

    Node* add(Node* a, E d) {
        if (a) a->add = g(a->add, d);
        return a;
    }

    std::deque<Node> buffer;
    Node* new_node(T v) { return &buffer.emplace_back(v, e); }
    void clear_node(Node*) {}

    Node* meld(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        if (c(top(b), top(a))) std::swap(a, b);
        eval(a);
        a->r = meld(a->r, b);
        std::swap(a->l, a->r);
        return a;
    }

    Node* pop(Node* a) {
        eval(a);
        auto res = meld(a->l, a->r);
        clear_node(a);
        return res;
    }
};

template <typename T, T INF>
T garsia_wachs(std::vector<T> s) {
    int n = (int)s.size();
    constexpr auto F = [](T a, T b) {
        return a + b;
    };
    constexpr auto G = [](T a, T b) {
        return a + b;
    };
    constexpr auto C = [](T a, T b) {
        return a < b;
    };
    using Heap = SkewHeap<T, T, decltype(F), decltype(G), decltype(C), INF, 0>;
    Heap heap(F, G, C);
    std::vector<typename Heap::Node*> hs(n, nullptr);
    std::vector<int> ls(n - 1), rs(n - 1);
    std::vector<T> cs(n - 1);

    using P = std::pair<T, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    for (int i = 0; i < n - 1; i++) {
        ls[i] = i - 1;
        rs[i] = i + 1;
        cs[i] = s[i] + s[i + 1];
        pq.emplace(cs[i], i);
    }

    T res = 0;
    for (int k = 0; k < n - 1; k++) {
        T c;
        int i;
        do {
            std::tie(c, i) = pq.top();
            pq.pop();
        } while (rs[i] < 0 || cs[i] != c);

        bool ml = false, mr = false;
        if (s[i] + heap.top(hs[i]) == c) {
            hs[i] = heap.pop(hs[i]);
            ml = true;
        } else if (s[i] + s[rs[i]] == c) {
            ml = mr = true;
        } else if (heap.top(hs[i]) + heap.worse_child(hs[i]) == c) {
            hs[i] = heap.pop(heap.pop(hs[i]));
        } else {
            hs[i] = heap.pop(hs[i]);
            mr = true;
        }

        res += c;
        hs[i] = heap.meld(hs[i], heap.new_node(c));

        if (ml) s[i] = INF;
        if (mr) s[rs[i]] = INF;

        if (ml && i > 0) {
            int j = ls[i];
            hs[j] = heap.meld(hs[j], hs[i]);
            rs[j] = rs[i];
            rs[i] = -1;
            ls[rs[j]] = j;
            i = j;
        }

        if (mr && rs[i] + 1 < n) {
            int j = rs[i];
            hs[i] = heap.meld(hs[i], hs[j]);
            rs[i] = rs[j];
            rs[j] = -1;
            ls[rs[i]] = i;
        }

        cs[i] = std::min({s[i] + s[rs[i]],
                          std::min(s[i], s[rs[i]]) + heap.top(hs[i]),
                          heap.top(hs[i]) + heap.worse_child(hs[i])});

        pq.emplace(cs[i], i);
    }
    return res;
}

