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

