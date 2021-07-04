template <class T>
struct range_vector {
    int min_index;
    vector<T> data;
    range_vector(int _min_index, int _max_index, T&& values = T())
        : data(_max_index - _min_index + 1, values), min_index(_min_index) {
        assert(_min_index <= _max_index + 1);
    }
    T& operator[](int i) { return data[i - min_index]; }
};
