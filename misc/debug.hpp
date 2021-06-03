#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace Debug {

// clang-format off
#define DEBUG_RED              "\x1B[0m\x1B[31m"
#define DEBUG_GREEN            "\x1B[0m\x1B[32m"
#define DEBUG_YELLOW           "\x1B[0m\x1B[33m"
#define DEBUG_BLUE             "\x1B[0m\x1B[34m"
#define DEBUG_MAGENTA          "\x1B[0m\x1B[35m"
#define DEBUG_CYAN             "\x1B[0m\x1B[36m"
#define DEBUG_WHITE            "\x1B[0m\x1B[37m"
#define DEBUG_RESET            "\x1B[0m"
#define DEBUG_BOLD_GREEN       "\x1B[0m\x1B[32;1m"
#define DEBUG_BOLD_YELLOW      "\x1B[0m\x1B[33;1m"
#define DEBUG_BOLD_BLUE        "\x1B[0m\x1B[34;1m"
#define DEBUG_BOLD_MAGENTA     "\x1B[0m\x1B[35;1m"
#define DEBUG_BOLD_CYAN        "\x1B[0m\x1B[36;1m"
#define DEBUG_BOLD_WHITE       "\x1B[0m\x1B[37;1m"
#define DEBUG_UNDERLINE        "\x1B[4m"
#define DEBUG_BODY             DEBUG_BOLD_CYAN
#define DEBUG_VAR              DEBUG_RED
#define DEBUG_PAREN            DEBUG_YELLOW
#define DEBUG_BRAC             DEBUG_GREEN
#define DEBUG_COMMA            DEBUG_BOLD_MAGENTA
#define DEBUG_CONTENT          DEBUG_BODY DEBUG_UNDERLINE
#define DEBUG_LINE             DEBUG_BOLD_YELLOW
#define DEBUG_LINENUM          DEBUG_BOLD_BLUE
    // clang-format on

#define SFINAE(x, ...)             \
    template <class, class = void> \
    struct x : std::false_type {}; \
    template <class T>             \
    struct x<T, std::void_t<__VA_ARGS__>> : std::true_type {}

    SFINAE(DefaultIO, decltype(std::cout << std::declval<T &>()));
    SFINAE(IsTuple, typename std::tuple_size<T>::type);
    SFINAE(Iterable, decltype(std::begin(std::declval<T>())));

    template <auto &os>
    struct Writer {
        static constexpr auto lbrac = DEBUG_BRAC "[";
        static constexpr auto rbrac = DEBUG_BRAC "]";
        static constexpr auto lparen = DEBUG_PAREN "(";
        static constexpr auto rparen = DEBUG_PAREN ")";
        static constexpr auto comma = DEBUG_COMMA ", ";
        template <class T>
        void Impl(T const &t) const {
            if constexpr (DefaultIO<T>::value) {
                os << DEBUG_CONTENT << t;
            } else if constexpr (Iterable<T>::value) {
                int i = 0;
                os << lbrac;
                for (auto &&x : t) ((i++) ? (os << comma, Impl(x)) : Impl(x));
                os << rbrac;
            } else if constexpr (IsTuple<T>::value) {
                os << lparen;
                std::apply(
                    [this](auto const &... args) {
                        int i = 0;
                        (((i++) ? (os << comma, Impl(args)) : Impl(args)), ...);
                    },
                    t);
                os << rparen;
            } else
                static_assert(IsTuple<T>::value, "No matching type for print");
        }
        template <class F, class... Ts>
        auto &operator()(F const &f, Ts const &... ts) const {
            return Impl(f), ((os << comma, Impl(ts)), ...), os << '\n', *this;
        }
    };

    template <auto &is>
    struct Reader {
        template <class T>
        auto &Rd(T &t) const {
            if constexpr (DefaultIO<T>::value)
                is >> t;
            else if constexpr (Iterable<T>::value)
                for (auto &x : t) Rd(x);
            else if constexpr (IsTuple<T>::value)
                std::apply([this](auto &... args) { (Rd(args), ...); }, t);
            else
                static_assert(IsTuple<T>::value, "No matching type for read");
            return *this;
        }
        template <class T>
        auto operator()(T t) const {
            Rd(t);
            return t;
        }
    };

#ifdef DEBUG
    #define debug(args...)                                \
        {                                                 \
            std::string _s = #args;                       \
            std::replace(_s.begin(), _s.end(), ',', ' '); \
            std::stringstream _ss(_s);                    \
            std::istream_iterator<std::string> _it(_ss);  \
            Debug::location_stats(__LINE__);              \
            Debug::err(_it, args);                        \
            Debug::clear_colours();                       \
        }

    void location_stats(int line) {
        std::cerr << DEBUG_LINE << "Line " << DEBUG_LINENUM << line << "\n"
                  << DEBUG_RESET;
    }
    void clear_colours() { std::cerr << DEBUG_RESET; }
    void err(std::istream_iterator<std::string> it) { std::ignore = it; }
    template <typename T, typename... Args>
    void err(std::istream_iterator<std::string> it, T a, Args... args) {
        std::cerr << DEBUG_VAR << *it << " = ";
        Writer<std::cerr>{}(a);
        err(++it, args...);
    }

    #define ASSERT(...) \
        if (not(__VA_ARGS__)) throw runtime_error(#__VA_ARGS__)
#else
    #define debug(...) 0
    #define ASSERT(...) 0
#endif

#undef DEBUG_RED
#undef DEBUG_GREEN
#undef DEBUG_YELLOW
#undef DEBUG_BLUE
#undef DEBUG_MAGENTA
#undef DEBUG_CYAN
#undef DEBUG_WHITE
#undef DEBUG_RESET
#undef DEBUG_BOLD_GREEN
#undef DEBUG_BOLD_YELLOW
#undef DEBUG_BOLD_BLUE
#undef DEBUG_BOLD_MAGENTA
#undef DEBUG_BOLD_CYAN
#undef DEBUG_BOLD_WHITE
#undef DEBUG_UNDERLINE
#undef DEBUG_BODY
#undef DEBUG_VAR
#undef DEBUG_PAREN
#undef DEBUG_BRAC
#undef DEBUG_COMMA
#undef DEBUG_CONTENT
#undef DEBUG_LINE
#undef DEBUG_LINENUM

}  // namespace Debug

