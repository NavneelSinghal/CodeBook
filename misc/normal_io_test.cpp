struct IO_TEST {
    static_assert(IO::is_default<int>::value);
    static_assert(!IO::is_iterable<int>::value);
    static_assert(!IO::is_applyable<int>::value);

    static_assert(IO::is_default<float>::value);
    static_assert(!IO::is_iterable<float>::value);
    static_assert(!IO::is_applyable<float>::value);

    static_assert(IO::is_default<double>::value);
    static_assert(!IO::is_iterable<double>::value);
    static_assert(!IO::is_applyable<double>::value);

    static_assert(IO::is_default<char>::value);
    static_assert(!IO::is_iterable<char>::value);
    static_assert(!IO::is_applyable<char>::value);

    static_assert(IO::is_default<long long>::value);
    static_assert(!IO::is_iterable<long long>::value);
    static_assert(!IO::is_applyable<long long>::value);

    static_assert(IO::is_default<std::string>::value);
    static_assert(IO::is_iterable<std::string>::value);  // note this
    static_assert(!IO::is_applyable<std::string>::value);

    static_assert(!IO::is_default<std::vector<int>>::value);
    static_assert(IO::is_iterable<std::vector<int>>::value);
    static_assert(!IO::is_applyable<std::vector<int>>::value);

    static_assert(!IO::is_default<std::array<int, 2>>::value);
    static_assert(IO::is_iterable<std::array<int, 2>>::value);
    static_assert(IO::is_applyable<std::array<int, 2>>::value);  // note this

    static_assert(
        !IO::is_default<std::tuple<int, char, std::vector<int>>>::value);
    static_assert(
        !IO::is_iterable<std::tuple<int, char, std::vector<int>>>::value);
    static_assert(
        IO::is_applyable<std::tuple<int, char, std::vector<int>>>::value);
};


