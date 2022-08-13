#include <stdbool.h>
#define FIRST_FALSE(T)                       \
    T first_false_##T(T l, T r, bool f(T)) { \
        for (--l, ++r; l + 1 < r;) {         \
            T m = l + (r - l) / 2;           \
            if (f(m))                        \
                l = m;                       \
            else                             \
                r = m;                       \
        }                                    \
        return r;                            \
    }
#define LAST_TRUE(T)                       \
    T last_true_##T(T l, T r, bool f(T)) { \
        for (--l, ++r; l + 1 < r;) {       \
            T m = l + (r - l) / 2;         \
            if (f(m))                      \
                l = m;                     \
            else                           \
                r = m;                     \
        }                                  \
        return l;                          \
    }

// sample usage

// typedef long long ll;
// FIRST_FALSE(ll);
// LAST_TRUE(ll);

