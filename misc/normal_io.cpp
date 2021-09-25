struct IO {
    IO() = default;
    IO(const IO&) = delete;
    IO(IO&&) = delete;
    IO& operator=(const IO&) = delete;
    IO& operator=(IO&&) = delete;
    ~IO() {}

    template <class T, class D = void>
    struct is_default {
        static constexpr bool value = false;
    };
    template <class T>
    struct is_default<T,
                      std::void_t<decltype(std::cout << std::declval<T>())>> {
        static constexpr bool value = true;
    };
    template <class T, class D = void>
    struct is_iterable {
        static constexpr bool value = false;
    };
    template <class T>
    struct is_iterable<T,
                       std::void_t<decltype(std::begin(std::declval<T>()))>> {
        static constexpr bool value = true;
    };
    template <class T, class D = void, class E = void>
    struct is_applyable {
        static constexpr bool value = false;
    };
    template <class T>
    struct is_applyable<T, std::void_t<typename std::tuple_size<T>::type>,
                        std::void_t<decltype(std::get<0>(std::declval<T>()))>> {
        static constexpr bool value = true;
    };

    template <class T>
    struct needs_newline {
        static constexpr bool value =
            (is_iterable<T>::value or is_applyable<T>::value) and
            (not is_default<T>::value);
    };

    template <typename T, typename U>
    struct any_needs_newline {};
    template <typename T>
    struct any_needs_newline<T, std::index_sequence<>> {
        static constexpr bool value = false;
    };
    template <typename T, std::size_t I, std::size_t... Is>
    struct any_needs_newline<T, std::index_sequence<I, Is...>> {
        static constexpr bool value =
            (needs_newline<decltype(std::get<I>(std::declval<T>()))>::value) ||
            any_needs_newline<T, std::index_sequence<Is...>>::value;
    };

    template <typename T_>
    IO& operator<<(T_&& x) {
        using T = typename std::remove_cv<typename std::remove_reference<T_>::type>::type;
        static_assert(is_default<T>::value or is_iterable<T>::value or
                      is_applyable<T>::value);
        if constexpr (is_default<T>::value) {
            std::cout << x;
        } else if constexpr (is_iterable<T>::value) {
            // strings are immune
            using E = decltype(*std::begin(x));
            constexpr char sep = needs_newline<E>::value ? '\n' : ' ';
            int i = 0;
            for (const auto& y : x) {
                if (i++) std::cout << sep;
                operator<<(y);
            }
        } else if constexpr (is_applyable<T>::value) {
            // strings are immune
            constexpr char sep =
                (any_needs_newline<
                    T, std::make_index_sequence<std::tuple_size_v<T>>>::value)
                    ? '\n'
                    : ' ';
            int i = 0;
            std::apply(
                [this, &sep, &i](auto const&... y) {
                    ((std::cout << (i++ ? sep : '\0'), this->operator<<(y)),
                     ...);
                },
                x);
        }
        return *this;
    }
    template <typename T>
    IO& operator>>(T& x) {
        static_assert(is_default<T>::value or is_iterable<T>::value or
                      is_applyable<T>::value);
        if constexpr (is_default<T>::value)
            std::cin >> x;
        else if constexpr (is_iterable<T>::value)
            for (auto& y : x) operator>>(y);
        else if constexpr (is_applyable<T>::value)
            std::apply([this](auto&... y) { ((operator>>(y)), ...); }, x);
        return *this;
    }

    IO* tie(std::nullptr_t) {
        std::cin.tie(nullptr);
        return this;
    }
    void sync_with_stdio(bool b) { std::cin.sync_with_stdio(b); }
};

IO io;
#define cin io
#define cout io

