namespace graph_representations {
    template <typename Data>
    struct edge_data {
        int to, nxt;
        Data d;
        constexpr edge_data() : to{}, nxt{}, d{} {}
        constexpr edge_data(int to_, int nxt_, Data d_)
            : to{to_}, nxt{nxt_}, d{d_} {}
    };
    struct edge {
        int to, nxt;
        constexpr edge() : to{}, nxt{} {}
        constexpr edge(int to_, int nxt_) : to{to_}, nxt{nxt_} {}
    };
    template <typename Data, int N, int M>
    struct graph {
        using edge_t = edge_data<Data>;
        constexpr graph() : cur_edges{} { head.fill(-1); }
        void add_edge(int u, int v, Data d) {
            edges[cur_edges] = {v, head[u], d};
            head[u] = cur_edges++;
        }
        struct edge_range {
            const edge_t* edges_array;
            const int start;
            struct edge_iterator {
                const edge_t* edges_array;
                int edge_index;
                explicit constexpr edge_iterator(const edge_t* edges_array_,
                                                 const int edge_index_)
                    : edges_array{edges_array_}, edge_index{edge_index_} {}
                constexpr const edge_iterator& operator++() {
                    edge_index = edges_array[edge_index].nxt;
                    return *this;
                }
                constexpr const edge_iterator operator++(int) {
                    auto temp = *this;
                    return operator++(), temp;
                }
                constexpr auto operator*() const {
                    return std::pair{edges_array[edge_index].to,
                                     edges_array[edge_index].d};
                }
                constexpr bool operator!=(const edge_iterator& it) const {
                    return edge_index != it.edge_index;
                }
                constexpr bool operator==(const edge_iterator& it) const {
                    return edge_index == it.edge_index;
                }
            };
            constexpr edge_iterator begin() {
                return edge_iterator(edges_array, start);
            }
            constexpr edge_iterator end() {
                return edge_iterator(edges_array, -1);
            }
            constexpr edge_iterator begin() const {
                return edge_iterator(edges_array, start);
            }
            constexpr edge_iterator end() const {
                return edge_iterator(edges_array, -1);
            }
        };
        edge_range operator[](int u) {
            return edge_range{edges.data(), head[u]};
        }
        edge_range operator[](int u) const {
            return edge_range{edges.data(), head[u]};
        }

       private:
        std::array<int, N> head;
        std::array<edge_t, M> edges;
        int cur_edges;
    };
    template <int N, int M>
    struct graph<void, N, M> {
        using edge_t = edge;
        constexpr graph() : cur_edges{} { head.fill(-1); }
        void add_edge(int u, int v) {
            edges[cur_edges] = {v, head[u]};
            head[u] = cur_edges++;
        }
        struct edge_range {
            const edge_t* edges_array;
            const int start;
            struct edge_iterator {
                const edge_t* edges_array;
                int edge_index;
                explicit constexpr edge_iterator(const edge_t* edges_array_,
                                                 const int edge_index_)
                    : edges_array{edges_array_}, edge_index{edge_index_} {}
                constexpr const edge_iterator& operator++() {
                    edge_index = edges_array[edge_index].nxt;
                    return *this;
                }
                constexpr const edge_iterator operator++(int) {
                    auto temp = *this;
                    return operator++(), temp;
                }
                constexpr auto operator*() const {
                    return edges_array[edge_index].to;
                }
                constexpr bool operator!=(const edge_iterator& it) const {
                    return edge_index != it.edge_index;
                }
                constexpr bool operator==(const edge_iterator& it) const {
                    return edge_index == it.edge_index;
                }
            };
            constexpr edge_iterator begin() {
                return edge_iterator(edges_array, start);
            }
            constexpr edge_iterator end() {
                return edge_iterator(edges_array, -1);
            }
            constexpr edge_iterator begin() const {
                return edge_iterator(edges_array, start);
            }
            constexpr edge_iterator end() const {
                return edge_iterator(edges_array, -1);
            }
        };
        edge_range operator[](int u) {
            return edge_range{edges.data(), head[u]};
        }
        edge_range operator[](int u) const {
            return edge_range{edges.data(), head[u]};
        }

       private:
        std::array<int, N> head;
        std::array<edge_t, M> edges;
        int cur_edges;
    };
    template <typename Data>
    struct graph<Data, -1, -1> {
        using edge_t = edge_data<Data>;
        graph(int n, int m) : head(n, -1), cur_edges{} { edges.reserve(m); }
        void add_edge(int u, int v, Data d) {
            edges.push_back({v, head[u], d});
            head[u] = cur_edges++;
        }
        struct edge_range {
            const edge_t* edges_array;
            const int start;
            struct edge_iterator {
                const edge_t* edges_array;
                int edge_index;
                explicit constexpr edge_iterator(const edge_t* edges_array_,
                                                 const int edge_index_)
                    : edges_array{edges_array_}, edge_index{edge_index_} {}
                constexpr const edge_iterator& operator++() {
                    edge_index = edges_array[edge_index].nxt;
                    return *this;
                }
                constexpr const edge_iterator operator++(int) {
                    auto temp = *this;
                    return operator++(), temp;
                }
                constexpr auto operator*() const {
                    return std::pair{edges_array[edge_index].to,
                                     edges_array[edge_index].d};
                }
                constexpr bool operator!=(const edge_iterator& it) const {
                    return edge_index != it.edge_index;
                }
                constexpr bool operator==(const edge_iterator& it) const {
                    return edge_index == it.edge_index;
                }
            };
            constexpr edge_iterator begin() {
                return edge_iterator(edges_array, start);
            }
            constexpr edge_iterator end() {
                return edge_iterator(edges_array, -1);
            }
            constexpr edge_iterator begin() const {
                return edge_iterator(edges_array, start);
            }
            constexpr edge_iterator end() const {
                return edge_iterator(edges_array, -1);
            }
        };
        edge_range operator[](int u) {
            return edge_range{edges.data(), head[u]};
        }
        edge_range operator[](int u) const {
            return edge_range{edges.data(), head[u]};
        }

       private:
        std::vector<int> head;
        std::vector<edge_t> edges;
        int cur_edges;
    };
    template <>
    struct graph<void, -1, -1> {
        using edge_t = edge;
        graph(int n, int m) : head(n, -1), cur_edges{} { edges.reserve(m); }
        void add_edge(int u, int v) {
            edges.push_back({v, head[u]});
            head[u] = cur_edges++;
        }
        struct edge_range {
            const edge_t* edges_array;
            const int start;
            struct edge_iterator {
                const edge_t* edges_array;
                int edge_index;
                explicit constexpr edge_iterator(const edge_t* edges_array_,
                                                 const int edge_index_)
                    : edges_array{edges_array_}, edge_index{edge_index_} {}
                constexpr const edge_iterator& operator++() {
                    edge_index = edges_array[edge_index].nxt;
                    return *this;
                }
                constexpr const edge_iterator operator++(int) {
                    auto temp = *this;
                    return operator++(), temp;
                }
                constexpr auto operator*() const {
                    return edges_array[edge_index].to;
                }
                constexpr bool operator!=(const edge_iterator& it) const {
                    return edge_index != it.edge_index;
                }
                constexpr bool operator==(const edge_iterator& it) const {
                    return edge_index == it.edge_index;
                }
            };
            constexpr edge_iterator begin() {
                return edge_iterator(edges_array, start);
            }
            constexpr edge_iterator end() {
                return edge_iterator(edges_array, -1);
            }
            constexpr edge_iterator begin() const {
                return edge_iterator(edges_array, start);
            }
            constexpr edge_iterator end() const {
                return edge_iterator(edges_array, -1);
            }
        };
        edge_range operator[](int u) {
            return edge_range{edges.data(), head[u]};
        }
        edge_range operator[](int u) const {
            return edge_range{edges.data(), head[u]};
        }

       private:
        std::vector<int> head;
        std::vector<edge_t> edges;
        int cur_edges;
    };
}  // namespace graph_representations

template <typename Data, int N, int M>
using fixed_size_graph_with_data_t = graph_representations::graph<Data, N, M>;
template <typename Data>
using graph_with_data_t = graph_representations::graph<Data, -1, -1>;
template <int N, int M>
using fixed_size_graph_t = graph_representations::graph<void, N, M>;
using graph_t = graph_representations::graph<void, -1, -1>;

// struct graph_edge_pointers {
//     struct edge {
//         int to, nxt, disable;  // to = other vertex,
//                                // nxt = index of the prev edge from cur vertex
//         edge(int to, int nxt) : to(to), nxt(nxt), disable(0) {}
//     };
// 
//     vector<int> head;
//     // head[i] = index of the last edge emanating from vertex i
//     vector<edge> edges;
//     vector<int> siz;
//     int cur_edges;
// 
//     graph_edge_pointers(int n, int m) : head(n, -1), siz(n), cur_edges(0) {
//         edges.reserve(m);
//     }
// 
//     // while adding (u, v), (v, u), we have i, i^1 as corresponding edges
//     void add_edge(int u, int v) {
//         edges.emplace_back(v, head[u]);
//         head[u] = cur_edges++;
//     }
// 
//     // example code
//     // function<bool(int, int, int)> dfs = [&] (int v, int p, int f) {
//     //     if (f <= 3) return true;
//     //     g.siz[v] = 1;
//     //     for (int i = g.head[v]; ~i; i = g.edges[i].nxt) {
//     //         if (g.edges[i].to != p && !g.edges[i].disable) {
//     //             int to = g.edges[i].to;
//     //             if (dfs(to, v, f))
//     //                 return true;
//     //             int siz_child = g.siz[to];
//     //             if (siz_child == fib[f - 1]) {
//     //                 g.edges[i].disable = 1;
//     //                 g.edges[i ^ 1].disable = 1;
//     //                 return dfs(to, to, f - 1) && dfs(v, v, f - 2);
//     //             } else if (siz_child == fib[f - 2]) {
//     //                 g.edges[i].disable = 1;
//     //                 g.edges[i ^ 1].disable = 1;
//     //                 return dfs(to, to, f - 2) && dfs(v, v, f - 1);
//     //             }
//     //             g.siz[v] += siz_child;
//     //         }
//     //     }
//     //     return false;
//     // };
// };
