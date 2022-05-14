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
        static constexpr bool has_data = !std::is_same_v<Data, void>;
        using edge_t = std::conditional_t<has_data, edge_data<Data>, edge>;
        constexpr graph() : cur_edges{} { head.fill(-1); }
        void clear(int n = N) {
            std::fill_n(std::begin(head), n, -1);
            cur_edges = 0;
        }
        template <class T>
        void add_edge(int u, int v, T d) noexcept {
            if constexpr (has_data) {
                edges[cur_edges] = {v, head[u], d};
            } else {
                throw "Graph doesn't handle edge data, do not use add_edge with data";
            }
            head[u] = cur_edges++;
        }
        void add_edge(int u, int v) noexcept {
            if constexpr (has_data) {
                throw "Graph handles edge data, do not use add_edge without data";
            } else {
                edges[cur_edges] = {v, head[u]};
            }
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
                    if constexpr (has_data) {
                        return std::pair{edges_array[edge_index].to,
                                         edges_array[edge_index].d};
                    } else {
                        return edges_array[edge_index].to;
                    }
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
        static constexpr bool has_data = !std::is_same_v<Data, void>;
        using edge_t = std::conditional_t<has_data, edge_data<Data>, edge>;
        graph(int n, int m) : head(n, -1), cur_edges{} { edges.reserve(m); }
        void clear() {
            std::fill(std::begin(head), std::end(head), -1);
            edges.clear();
            cur_edges = 0;
        }
        template <class T>
        void add_edge(int u, int v, T d) noexcept {
            if constexpr (has_data) {
                edges.push_back({v, head[u], d});
            } else {
                throw "Graph doesn't handle edge data, do not use add_edge with data";
            }
            head[u] = cur_edges++;
        }
        void add_edge(int u, int v) noexcept {
            if constexpr (has_data) {
                throw "Graph handles edge data, do not use add_edge without data";
            } else {
                edges.push_back({v, head[u]});
            }
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
                    if constexpr (has_data) {
                        return std::pair{edges_array[edge_index].to,
                                         edges_array[edge_index].d};
                    } else {
                        return edges_array[edge_index].to;
                    }
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

