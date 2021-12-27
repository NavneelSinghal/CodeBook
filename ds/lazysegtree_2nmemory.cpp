// consider trying to switch to alternative query and updates to make it slightly faster
// doesn't always help

// clang-format off
template <class Base,
          class Node,
          class Update,
          class MakeNode,
          class CombineNodes,
          class ApplyUpdate,
          class ComposeUpdates = std::nullptr_t,
          class CheckLazy = std::nullptr_t>
struct lazy_segtree {
    static constexpr bool is_lazy =
        !std::is_same<ComposeUpdates, std::nullptr_t>::value;
    static constexpr bool is_check_lazy = !std::is_same<CheckLazy, std::nullptr_t>::value;

   public:
    template <typename... T>
    explicit lazy_segtree(int n, const Base& id_base, T... args)
        : lazy_segtree(std::vector<Base>(n, id_base), args...) {}
    explicit lazy_segtree(const std::vector<Base>& v,
                          const Node& _id_node,
                          const MakeNode& _make_node,
                          const CombineNodes& _combine,
                          const Update& _id_update,
                          const ApplyUpdate& _apply_update,
                          const ComposeUpdates& _compose_updates = nullptr,
                          const CheckLazy& _check_lazy = nullptr)
        : _n(int(v.size())),
          make_node(_make_node),
          combine(_combine),
          id_node(_id_node),
          apply_update(_apply_update),
          id_update(_id_update),
          compose_updates(_compose_updates),
          check_lazy(_check_lazy) {
        build(v);
    }

    void build(const std::vector<Base>& v) {
        _n = int(v.size());
        log = 0;
        while ((2 * _n - 1) >> log > 1) ++log;
        d = std::vector<Node>(2 * _n, id_node);
        if constexpr (is_lazy) lz = std::vector<Update>(_n, id_update);
        for (int i = 0; i < _n; i++) d[_n + i] = make_node(v[i], i);
        for (int i = _n - 1; i >= 1; i--) update(i);
    }
    
    void set(int p, Node x) {
        p += _n;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; ++i) update(p >> i);
    }

    Node get(int p) {
        p += _n;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }
/*
    auto roots_xor_correct_order = [&](int l, int r) {
        int ans = 0;
        //if (l == r) return ans;
        l += n - 1, r += n;
        const int lg = __lg(r ^ l);
        const int lg_mask = (1 << lg) - 1;
        const int l_iter = lg - __builtin_popcount(l & lg_mask);
        for (int i = 0; i < l_iter; ++i) {
            ++(l >>= __builtin_ctz(~l));
            ans ^= (l);
        }
        const int r_iter = __builtin_popcount(r & lg_mask);
        int r_suffix = r & ((1 << lg) - 1);
        for (int i = 0; i < r_iter; ++i) {
            const int lg_suffix = __lg(r_suffix);
            const int R = r >> lg_suffix;
            ans ^= (R - 1);
            r_suffix ^= 1 << lg_suffix;
        }
        return ans;
    };
    auto roots_xor_reverse_order = [&](int l, int r) {
        int ans = 0;
        //if (l == r) return ans;
        l += n - 1, r += n;
        const int lg = __lg(r ^ l);
        const int lg_mask = (1 << lg) - 1;
        const int r_iter = __builtin_popcount(r & lg_mask);
        for (int i = 0; i < r_iter; ++i) {
            r >>= __builtin_ctz(r);
            ans ^= (--r);
        }
        const int l_iter = lg - __builtin_popcount(l & lg_mask);
        int l_suffix = (l & lg_mask) ^ lg_mask;
        for (int i = 0; i < l_iter; ++i) {
            const int lg_suffix = __lg(l_suffix);
            const int L = l >> lg_suffix;
            ans ^= (L + 1);
            l_suffix ^= 1 << lg_suffix;
        }
        return ans;
    };
*/

    Node query(int l, int r) {
        if (l == r) return id_node;
        l += _n, r += _n;
        if constexpr (is_lazy) {
            int l_ctz = __builtin_ctz(l);
            int r_ctz = __builtin_ctz(r);
            for (int i = log; i > l_ctz; --i) push(l >> i);
            for (int i = log; i > r_ctz; --i) push((r - 1) >> i);
        }
        Node sm = id_node;
        --l;
        const int lg = __lg(r ^ l);
        const int lg_mask = (1 << lg) - 1;
        const int l_iter = lg - __builtin_popcount(l & lg_mask);
        for (int i = 0; i < l_iter; ++i) {
            ++(l >>= __builtin_ctz(~l));
            sm = combine(sm, d[l]);
        }
        const int r_iter = __builtin_popcount(r & lg_mask);
        int r_suffix = r & ((1 << lg) - 1);
        for (int i = 0; i < r_iter; ++i) {
            const int lg_suffix = __lg(r_suffix);
            const int R = r >> lg_suffix;
            sm = combine(sm, d[R - 1]);
            r_suffix ^= 1 << lg_suffix;
        }
        return sm;
    }
    
    Node all_query() const { return query(0, _n); }
    
    void update(int p, Update f) {
        p += _n;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = apply_update(f, d[p]);
        for (int i = 1; i <= log; ++i) update(p >> i);
    }
    
    void update(int l, int r, Update f) {
        if (l == r) return;
        l += _n, r += _n;
        const int l_ctz = __builtin_ctz(l);
        const int r_ctz = __builtin_ctz(r);
        if constexpr (is_lazy) {
            for (int i = log; i > l_ctz; --i) push(l >> i);
            for (int i = log; i > r_ctz; --i) push((r - 1) >> i);
        }
        {
            const int l2 = l, r2 = r;
            --l;
            const int lg = __lg(r ^ l);
            const int lg_mask = (1 << lg) - 1;
            const int l_iter = lg - __builtin_popcount(l & lg_mask);
            for (int i = 0; i < l_iter; ++i) {
                ++(l >>= __builtin_ctz(~l));
                all_apply(l, f);
            }
            const int r_iter = __builtin_popcount(r & lg_mask);
            int r_suffix = r & ((1 << lg) - 1);
            for (int i = 0; i < r_iter; ++i) {
                const int lg_suffix = __lg(r_suffix);
                const int R = r >> lg_suffix;
                all_apply(R - 1, f);
                r_suffix ^= 1 << lg_suffix;
            }
            l = l2, r = r2;
        }
        for (int i = l_ctz + 1; i <= log; ++i) update(l >> i);
        for (int i = r_ctz + 1; i <= log; ++i) update((r - 1) >> i);
    }

    template <class G>
    int max_right(int l, G g) {
        // assert(0 <= l && l <= _n);
        // assert(g(id_node));

        if (l == _n) return _n;

        l += _n;
        int r = 2 * _n;
        
        const int l_ctz = __builtin_ctz(l);
        const int r_ctz = __builtin_ctz(r);
        
        if constexpr (is_lazy) {
            for (int i = log; i > l_ctz; --i) push(l >> i);
            for (int i = log; i > r_ctz; --i) push((r - 1) >> i);
        }
        
        Node sm = id_node;

        --l;
        int i = -1;
        const int lg = __lg(r ^ l);
        const int lg_mask = (1 << lg) - 1;
        const int l_iter = lg - __builtin_popcount(l & lg_mask);
        for (int it = 0; it < l_iter; ++it) {
            ++(l >>= __builtin_ctz(~l));
            int root = l;
            auto c = combine(sm, d[root]);
            if (!g(c)) {
                i = root;
                break;
            }
            sm = c;
        }

        if (i == -1) {
            const int r_iter = __builtin_popcount(r & lg_mask);
            int r_suffix = r & ((1 << lg) - 1);
            for (int it = 0; it < r_iter; ++it) {
                const int lg_suffix = __lg(r_suffix);
                const int R = r >> lg_suffix;
                r_suffix ^= 1 << lg_suffix;
                const int root = R - 1;
                auto c = combine(sm, d[root]);
                if (!g(c)) {
                    i = root;
                    break;
                }
                sm = c;
            }
        }

        if (i == -1) return _n;
        while (i < _n) {
            push(i);
            i = 2 * i;
            auto c = combine(sm, d[i]);
            if (g(c)) {
                sm = c;
                ++i;
            }
        }
        return i - _n;
    }

    template <class G>
    int min_left(int r, G g) {
        // assert(0 <= r && r <= _n);
        // assert(g(id_node));
        if (r == 0) return 0;
        r += _n;
        int l = _n;
       
        const int l_ctz = __builtin_ctz(l);
        const int r_ctz = __builtin_ctz(r);
        
        if constexpr (is_lazy) {
            for (int i = log; i > l_ctz; --i) push(l >> i);
            for (int i = log; i > r_ctz; --i) push((r - 1) >> i);
        }
        
        --l;
        Node sm = id_node;
        int i = -1;
        const int lg = __lg(r ^ l);
        const int lg_mask = (1 << lg) - 1;
        const int r_iter = __builtin_popcount(r & lg_mask);
        for (int it = 0; it < r_iter; ++it) {
            r >>= __builtin_ctz(r);
            const int root = --r;
            auto c = combine(d[root], sm);
            if (!g(c)) {
                i = root;
                break;
            }
            sm = c;
        }

        if (i == -1) {
            const int l_iter = lg - __builtin_popcount(l & lg_mask);
            int l_suffix = (l & lg_mask) ^ lg_mask;
            for (int it = 0; it < l_iter; ++it) {
                const int lg_suffix = __lg(l_suffix);
                const int L = l >> lg_suffix;
                l_suffix ^= 1 << lg_suffix;
                const int root = L + 1;
                auto c = combine(d[root], sm);
                if (!g(c)) {
                    i = root;
                    break;
                }
                sm = c;
            }
        }

        if (i == -1) return 0;
        while (i < _n) {
            push(i);
            i = 2 * i + 1;
            auto c = combine(d[i], sm);
            if (g(c)) {
                sm = c;
                --i;
            }
        }
        return i + 1 - _n;
    }

   private:
    int _n, log;
    std::vector<Node> d;
    std::vector<Update> lz;
    MakeNode make_node;
    CombineNodes combine;
    Node id_node;
    ApplyUpdate apply_update;
    Update id_update;
    ComposeUpdates compose_updates;
    CheckLazy check_lazy;

    void update(int k) { d[k] = combine(d[2 * k], d[2 * k + 1]); }
    void all_apply(int k, Update f) {
        d[k] = apply_update(f, d[k]);
        if constexpr (is_lazy)
            if (k < _n) lz[k] = compose_updates(f, lz[k]);
    }
    void push(int k) {
        if constexpr (is_check_lazy) {
            if (!check_lazy(lz[k])) return;
        }
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = id_update;
    }
};
// clang-format on

// // clang-format off
// template <class Base,
//           class Node,
//           class Update,
//           class MakeNode,
//           class CombineNodes,
//           class ApplyUpdate,
//           class ComposeUpdates = std::nullptr_t,
//           class CheckLazy = std::nullptr_t>
// struct lazy_segtree {
//     static constexpr bool is_lazy =
//         !std::is_same<ComposeUpdates, std::nullptr_t>::value;
//     static constexpr bool is_check_lazy = !std::is_same<CheckLazy, std::nullptr_t>::value;
// 
//    public:
//     template <typename... T>
//     explicit lazy_segtree(int n, const Base& id_base, T... args)
//         : lazy_segtree(std::vector<Base>(n, id_base), args...) {}
//     explicit lazy_segtree(const std::vector<Base>& v,
//                           const Node& _id_node,
//                           const MakeNode& _make_node,
//                           const CombineNodes& _combine,
//                           const Update& _id_update,
//                           const ApplyUpdate& _apply_update,
//                           const ComposeUpdates& _compose_updates = nullptr,
//                           const CheckLazy& _check_lazy = nullptr)
//         : _n(int(v.size())),
//           make_node(_make_node),
//           combine(_combine),
//           id_node(_id_node),
//           apply_update(_apply_update),
//           id_update(_id_update),
//           compose_updates(_compose_updates),
//           check_lazy(_check_lazy) {
//         build(v);
//     }
// 
//     void build(const std::vector<Base>& v) {
//         _n = int(v.size());
//         log = 0;
//         while ((2 * _n - 1) >> log > 1) ++log;
//         d = std::vector<Node>(2 * _n, id_node);
//         if constexpr (is_lazy) lz = std::vector<Update>(_n, id_update);
//         for (int i = 0; i < _n; i++) d[_n + i] = make_node(v[i], i);
//         for (int i = _n - 1; i >= 1; i--) update(i);
//     }
//     
//     void set(int p, Node x) {
//         p += _n;
//         if constexpr (is_lazy)
//             for (int i = log; i >= 1; i--) push(p >> i);
//         d[p] = x;
//         for (int i = 1; i <= log; ++i) update(p >> i);
//     }
// 
//     Node get(int p) {
//         p += _n;
//         if constexpr (is_lazy)
//             for (int i = log; i >= 1; i--) push(p >> i);
//         return d[p];
//     }
// 
//     Node query(int l, int r) {
//         if (l == r) return id_node;
//         l += _n, r += _n;
//         if constexpr (is_lazy) {
//             int l_ctz = __builtin_ctz(l);
//             int r_ctz = __builtin_ctz(r);
//             for (int i = log; i > l_ctz; --i) push(l >> i);
//             for (int i = log; i > r_ctz; --i) push((r - 1) >> i);
//         }
//         Node sml = id_node, smr = id_node;
//         while (l < r) {
//             if (l & 1) sml = combine(sml, d[l++]);
//             if (r & 1) smr = combine(d[--r], smr);
//             l >>= 1, r >>= 1;
//         }
//         return combine(sml, smr);
//         // Node sm = id_node;
//         // --l;
//         // const int lg = __lg(r ^ l);
//         // for (int h = 0; h < lg; ++h) {
//         //     const int L = (l >> h) + 1;
//         //     if (L & 1) sm = combine(sm, d[L]);
//         // }
//         // for (int h = lg - 1; h >= 0; --h) {
//         //     const int R = r >> h;
//         //     if (R % 2) sm = combine(sm, d[R - 1]);
//         // }
//         // return sm;
//     }
//     
//     Node all_query() const { return query(0, _n); }
//     
//     void update(int p, Update f) {
//         p += _n;
//         if constexpr (is_lazy)
//             for (int i = log; i >= 1; i--) push(p >> i);
//         d[p] = apply_update(f, d[p]);
//         for (int i = 1; i <= log; ++i) update(p >> i);
//     }
//     
//     void update(int l, int r, Update f) {
//         if (l == r) return;
//         l += _n, r += _n;
//         const int l_ctz = __builtin_ctz(l);
//         const int r_ctz = __builtin_ctz(r);
//         if constexpr (is_lazy) {
//             for (int i = log; i > l_ctz; --i) push(l >> i);
//             for (int i = log; i > r_ctz; --i) push((r - 1) >> i);
//         }
//         {
//             const int l2 = l, r2 = r;
//             while (l < r) {
//                 if (l & 1) all_apply(l++, f);
//                 if (r & 1) all_apply(--r, f);
//                 l >>= 1, r >>= 1;
//             }
//             // --l;
//             // const int lg = __lg(r ^ l);
//             // for (int h = 0; h < lg; ++h) {
//             //     const int L = (l >> h) + 1;
//             //     if (L & 1) all_apply(L, f);
//             // }
//             // for (int h = lg - 1; h >= 0; --h) {
//             //     const int R = r >> h;
//             //     if (R & 1) all_apply(R - 1, f);
//             // }
//             l = l2, r = r2;
//         }
//         for (int i = l_ctz + 1; i <= log; ++i) update(l >> i);
//         for (int i = r_ctz + 1; i <= log; ++i) update((r - 1) >> i);
//     }
// 
//     template <class G>
//     int max_right(int l, G g) {
//         // assert(0 <= l && l <= _n);
//         // assert(g(id_node));
// 
//         if (l == _n) return _n;
// 
//         l += _n;
//         int r = 2 * _n;
//         
//         const int l_ctz = __builtin_ctz(l);
//         const int r_ctz = __builtin_ctz(r);
//         
//         if constexpr (is_lazy) {
//             for (int i = log; i > l_ctz; --i) push(l >> i);
//             for (int i = log; i > r_ctz; --i) push((r - 1) >> i);
//         }
//         
//         Node sm = id_node;
// 
//         --l;
//         int i = -1;
//         int lg = __lg(r ^ l);
//         for (int h = 0; h < lg; ++h) {
//             int L = (l >> h) + 1;
//             if (L & 1) {
//                 // ans.push_back(L);
//                 int root = L;
//                 auto c = combine(sm, d[root]);
//                 if (!g(c)) {
//                     i = root;
//                     break;
//                 }
//                 sm = c;
//             }
//         }
//         
//         if (i == -1)
//             for (int h = lg - 1; h >= 0; --h) {
//                 int R = r >> h;
//                 if (R & 1) {
//                     // ans.push_back(R - 1);
//                     int root = R - 1;
//                     auto c = combine(sm, d[root]);
//                     if (!g(c)) {
//                         i = root;
//                         break;
//                     }
//                     sm = c;
//                 }
//             }
//         
//         if (i == -1) return _n;
//         while (i < _n) {
//             push(i);
//             i = 2 * i;
//             auto c = combine(sm, d[i]);
//             if (g(c)) {
//                 sm = c;
//                 ++i;
//             }
//         }
//         return i - _n;
//     }
// 
//     template <class G>
//     int min_left(int r, G g) {
//         // assert(0 <= r && r <= _n);
//         // assert(g(id_node));
//         if (r == 0) return 0;
//         r += _n;
//         int l = _n;
//        
//         const int l_ctz = __builtin_ctz(l);
//         const int r_ctz = __builtin_ctz(r);
//         
//         if constexpr (is_lazy) {
//             for (int i = log; i > l_ctz; --i) push(l >> i);
//             for (int i = log; i > r_ctz; --i) push((r - 1) >> i);
//         }
//         
//         --l;
//         Node sm = id_node;
//         int i = -1;
//         int lg = __lg(r ^ l);
//         for (int h = 0; h < lg; ++h) {
//             int R = r >> h;
//             if (R & 1) {
//                 int root = R - 1;
//                 auto c = combine(d[root], sm);
//                 if (!g(c)) {
//                     i = root;
//                     break;
//                 }
//                 sm = c;
//             }
//         }
// 
//         if (i == -1)
//             for (int h = lg - 1; h >= 0; --h) {
//                 int L = (l >> h) + 1;
//                 if (L & 1) {
//                     int root = L;
//                     auto c = combine(d[root], sm);
//                     if (!g(c)) {
//                         i = root;
//                         break;
//                     }
//                     sm = c;
//                 }
//             }
// 
//         if (i == -1) return 0;
//         while (i < _n) {
//             push(i);
//             i = 2 * i + 1;
//             auto c = combine(d[i], sm);
//             if (g(c)) {
//                 sm = c;
//                 --i;
//             }
//         }
//         return i + 1 - _n;
//     }
// 
//    private:
//     int _n, log;
//     std::vector<Node> d;
//     std::vector<Update> lz;
//     MakeNode make_node;
//     CombineNodes combine;
//     Node id_node;
//     ApplyUpdate apply_update;
//     Update id_update;
//     ComposeUpdates compose_updates;
//     CheckLazy check_lazy;
// 
//     void update(int k) { d[k] = combine(d[2 * k], d[2 * k + 1]); }
//     void all_apply(int k, Update f) {
//         d[k] = apply_update(f, d[k]);
//         if constexpr (is_lazy)
//             if (k < _n) lz[k] = compose_updates(f, lz[k]);
//     }
//     void push(int k) {
//         if constexpr (is_check_lazy) {
//             if (!check_lazy(lz[k])) return;
//         }
//         all_apply(2 * k, lz[k]);
//         all_apply(2 * k + 1, lz[k]);
//         lz[k] = id_update;
//     }
// };
// // clang-format on
// 
