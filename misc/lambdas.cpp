#include <iostream>
#include <vector>
#include <type_traits>

// this can be used for function pointer stuff
template <auto A, auto B = nullptr>
void f() {}

template <bool is_lazy, typename A, typename B = void*>
void f(A a, B b = nullptr) {
    a();
    if constexpr (is_lazy) b();
}

template <bool is_lazy, typename A, typename B = void*>
struct S {
    int n;
    A a;
    B b;
    S(int n_, A a_) : n(n_), a(a_) {}
    S(int n_, A a_, B b_) : n(n_), a(a_), b(b_) {}
    void operator()() {
        std::cout << n << '\n';
        a();
        if constexpr (is_lazy) b();
    }
};

template <typename A>
S(int, A) -> S<false, A>;

template <typename A, typename B>
S(int, A, B) -> S<true, A, B>;

// struct null_type {};

template <class S, class F = std::nullptr_t>
struct SegTree {
    static constexpr bool is_lazy = !std::is_same<F, std::nullptr_t>::value;
    int n;
    S s;
    F f;
    SegTree(int n_, S s_) : n(n_), s(s_) {}
    SegTree(std::vector<int> a_, S s_) : n((int)a_.size()), s(s_) {}
    SegTree(int n_, S s_, F f_) : n(n_), s(s_), f(f_) {}
    void operator()() {
        s();
        if constexpr (is_lazy) f();
    }
};

// struct MyS {};
// struct MyF {};

int main() {
    int x = 0;
    auto a = [&]() {
        ++x;
        std::cout << x << " a()\n";
    };
    auto b = [&]() {
        ++x;
        std::cout << x << " b()\n";
    };
    // f<true, decltype(a), decltype(b)>(a, b);
    f<true>(a, b);
    f<false>(a);
    S s(1, a);
    S t(1, a, b);
    // S<false, decltype(a)> s(1, a);
    s();
    t();
    //SegTree st1{MyS()};
    //SegTree st2{MyS(), MyF()};
    SegTree st1(1, a);
    SegTree st2(2, a, b);
    st1();
    st2();
}
