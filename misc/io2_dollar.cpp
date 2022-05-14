#include "bits/stdc++.h"

struct input {
    template <class T>
    operator T() {
        T x;
        std::cin >> x;
        return x;
    }

    template <class T, int num>
    struct tuple_type {
        using type = decltype(std::tuple_cat(
            std::tuple<T>{}, typename tuple_type<T, num - 1>::type{}));
    };
    template <class T>
    struct tuple_type<T, 0> {
        using type = std::tuple<>;
    };

    template <class T, int num>
    auto get() {
        typename tuple_type<T, num>::type t;
        std::apply([](auto&... x) { ((std::cin >> x), ...); }, t);
        return t;
    }

    template <class... T>
    auto get() {
        std::tuple<T...> t;
        std::apply([](auto&... x) { ((std::cin >> x), ...); }, t);
        return t;
    }
} $;

int main() {
    int x = $;
    auto [s, t] = $.get<std::string, 2>();
    auto [S, T] = $.get<std::string, int>();
}

