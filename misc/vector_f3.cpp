template <std::size_t N>
struct VectorF3 {
    using value_type = std::array<std::bitset<N>, 3>;
    value_type data;
    constexpr VectorF3() { data[0].set(), data[1].reset(), data[2].reset(); }
    constexpr VectorF3(const value_type& data_) : data{data_} {}
    int get(int id) const {
        for (int i = 0; i < 3; ++i)
            if (data[i][id]) return i;
        return -1;
    }
    void set(int id, int val) {
        val = (val % 3 + 3) % 3;
        for (int i = 0; i < 3; ++i) data[i][id] = i == val;
    }
    void fill(int val) {
        val = (val % 3 + 3) % 3;
        for (int i = 0; i < 3; ++i)
            if (i == val)
                data[i].set();
            else
                data[i].reset();
    }
    VectorF3 operator-() const {
        auto a{*this};
        std::swap(a.data[1], a.data[2]);
        return a;
    }
    VectorF3 operator+(const VectorF3& a) const {
        VectorF3 b{value_type{}};
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                b.data[(i + j) % 3] |= data[i] & a.data[j];
        return b;
    }
    VectorF3 operator-(const VectorF3& a) const { return *this + (-a); }
    VectorF3 operator+=(const VectorF3& a) { return *this = *this + a; }
    VectorF3 operator-=(const VectorF3& a) { return *this = *this + (-a); }
    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
    VectorF3& operator*=(T x) {
        x = (x % 3 + 3) % 3;
        if (x == 0)
            data[0].reset(), data[1].reset(), data[2].reset();
        else if (x == 2)
            swap(data[1], data[2]);
        return *this;
    }
    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
    friend VectorF3 operator*(T x, const VectorF3& a) {
        auto A{a};
        return A *= x;
    }
};

