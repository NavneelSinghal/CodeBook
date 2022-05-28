template <class T>
struct PersistentArray {
    PersistentArray(const std::vector<T>& v) : d{{v, 0}} {}

    // returns value of latest version
    T get(int version, int index) { return d[version].get(index, d); }

    // returns value of latest version
    T get(int index) { return d.back().get(index, d); }

    // returns new version number
    int set(int version, int index, T value) {
        int new_index = (int)d.size();
        d.push_back(d[version].set(index, value, new_index));
        return new_index;
    }

    // returns new version number
    int set(int index, T value) {
        int new_index = (int)d.size();
        d.push_back(d.back().set(index, value, new_index));
        return new_index;
    }

    int total_versions() { return (int)d.size(); }

   private:
    struct Wrapper;
    struct Diff {
        int index;
        T value;
        int parent_index;
    };
    using Node = std::variant<std::vector<T>, Diff>;
    using Container = std::vector<Wrapper>;

    struct Wrapper {
        Node node;
        int self_index;
        // for a single persistent array, the container c should be the same
        T get(int i, Container& c) {
            reroot(c);
            return std::visit(
                [&](auto&& data) {
                    using type = std::decay_t<decltype(data)>;
                    if constexpr (std::is_same_v<type, std::vector<T>>) {
                        return data[i];
                    } else {
                        // assert(false);
                        // after rerooting, this is unreachable
                        return T{};
                    }
                },
                node);
        }
        Wrapper set(int i, T v, int new_self_index) {
            return Wrapper{Diff{i, v, self_index}, new_self_index};
        }
        void reroot(Container& c) {
            std::visit(
                [&](auto&& data) {
                    using type = std::decay_t<decltype(data)>;
                    if constexpr (std::is_same_v<type, std::vector<T>>) {
                        return;
                    } else {
                        // index of diff
                        auto i = data.index;
                        // value after diff
                        // using a reference will be UB, so either use boxed T
                        // or T where copying is cheap
                        auto v = data.value;
                        auto parent = data.parent_index;
                        // reverse linked list
                        c[parent].reroot(c);
                        // now set parent as the child of this
                        node = std::vector<T>{};
                        std::visit(
                            [&](auto&& p) {
                                using type_2 = std::decay_t<decltype(p)>;
                                if constexpr (std::is_same_v<type_2,
                                                             std::vector<T>>) {
                                    std::swap(v, p[i]);
                                    std::swap(std::get<std::vector<T>>(node),
                                              p);
                                } else {
                                    // assert(false);
                                }
                            },
                            c[parent].node);
                        c[parent].node = Diff{i, v, self_index};
                    }
                },
                node);
        }
    };
    Container d;
};

struct PersistentDSU {
    int n;
    PersistentArray<int> parent;
    std::vector<int> versions;
    PersistentDSU(int n_)
        : n{n_}, parent(std::vector<int>(n_, -1)), versions{0} {}
    // need to update versions[version] in root() as well to keep amortization
    // no path compression in what follows:
    int root(int version, int u) { return root_(versions[version], u); }
    int root_(int internal_version, int u) {
        int p = parent.get(internal_version, u);
        if (p < 0) return u;
        return root_(internal_version, p);
    }
    bool join(int version, int u, int v) {
        int internal_version = versions[version];
        u = root_(internal_version, u);
        v = root_(internal_version, v);
        if (u != v) {
            int pu = parent.get(internal_version, u);
            int pv = parent.get(internal_version, v);
            if (pu > pv) std::swap(u, v);
            int new_version_1 = parent.set(internal_version, u, pu + pv);
            int new_version_2 = parent.set(new_version_1, v, u);
            versions.push_back(new_version_2);
            return true;
        } else {
            add_dummy_version(internal_version);
            return false;
        }
    }
    void add_sentinel_version() { versions.push_back(-1); }
    void add_dummy_version() { add_dummy_version(parent.total_versions() - 1); }
    void add_dummy_version(int internal_version) {
        versions.push_back(
            parent.set(internal_version, 0, parent.get(internal_version, 0)));
    }
};

