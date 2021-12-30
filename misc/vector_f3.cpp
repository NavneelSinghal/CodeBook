template <std::size_t N>
struct VectorF3 {
    using B = std::bitset<N>;
    using value_type = std::array<std::bitset<N>, 3>;
    value_type data;

    struct reference {
        friend struct VectorF3;
        using value_type = std::array<typename B::reference, 3>;

       private:
        constexpr reference();
        value_type indices;

       public:
        constexpr reference(VectorF3& v, int id) noexcept
            : indices{v.data[0][id], v.data[1][id], v.data[2][id]} {}
        constexpr reference& operator=(int x) noexcept {
            for (int i = 0; i < 3; ++i) indices[i] = i == x;
            return *this;
        }
        constexpr reference& operator=(const reference& x) noexcept {
            indices = x.indices;
            return *this;
        }
        constexpr operator int() const noexcept {
            for (int i = 0; i < 3; ++i)
                if (indices[i]) return i;
            return 0;
        }
    };
    friend struct reference;

    constexpr VectorF3() { data[0].set(), data[1].reset(), data[2].reset(); }
    constexpr VectorF3(const value_type& data_) : data{data_} {}
    constexpr reference operator[](int id) { return reference(*this, id); }
    constexpr void fill(int val) {
        for (int i = 0; i < 3; ++i)
            if (i == val)
                data[i].set();
            else
                data[i].reset();
    }
    constexpr VectorF3 operator-() const {
        auto a{*this};
        std::swap(a.data[1], a.data[2]);
        return a;
    }
    constexpr VectorF3 operator+(const VectorF3& a) const {
        VectorF3 b{value_type{}};
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                b.data[(i + j) % 3] |= data[i] & a.data[j];
        return b;
    }
    constexpr VectorF3 operator-(const VectorF3& a) const {
        return *this + (-a);
    }
    constexpr VectorF3 operator+=(const VectorF3& a) {
        return *this = *this + a;
    }
    constexpr VectorF3 operator-=(const VectorF3& a) {
        return *this = *this + (-a);
    }
    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
    constexpr VectorF3& operator*=(T x) {
        if (x == 0)
            data[0].reset(), data[1].reset(), data[2].reset();
        else if (x == 2)
            swap(data[1], data[2]);
        return *this;
    }
    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
    constexpr friend VectorF3 operator*(T x, const VectorF3& a) {
        auto A{a};
        return A *= x;
    }
};

