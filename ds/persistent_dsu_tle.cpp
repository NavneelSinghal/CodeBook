template <class T>
struct PersistentArray {
    using Array = std::vector<T>;

    PersistentArray(const Array& data_)
        : data{data_}, d{{true, -1, T{}, -1, 0}} {}

    // returns value of latest version
    T get(int version, int index) { return d[version].get(index, d, data); }

    // returns value of latest version
    T get(int index) { return d.back().get(index, d, data); }

    // returns new version number
    int set(int version, int index, T value) {
        int new_pos = (int)d.size();
        d.push_back(d[version].set(index, value, new_pos));
        return new_pos;
    }

    // returns new version number
    int set(int index, T value) {
        int new_pos = (int)d.size();
        d.push_back(d.back().set(index, value, new_pos));
        return new_pos;
    }

    int total_versions() { return (int)d.size(); }

   private:
    struct Node;
    using Container = std::vector<Node>;
    struct Node {
        bool is_root;
        int index;
        T value;
        int parent_pos;
        int self_pos;
        // for a single persistent array, the container c should be the same
        T get(int i, Container& c, Array& a) {
            reroot(c, a);
            return a[i];
        }
        Node set(int i, T v, int new_self_pos) {
            return Node{false, i, v, self_pos, new_self_pos};
        }
        void reroot(Container& c, Array& a) {
            if (!is_root) {
                auto i = index;
                auto v = value;
                auto& p = c[parent_pos];
                p.reroot(c, a);
                is_root = true;
                std::swap(v, a[i]);
                p.is_root = false;
                p.index = i;
                p.value = v;
                p.parent_pos = self_pos;
            }
        }
    };
    Array data;
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

