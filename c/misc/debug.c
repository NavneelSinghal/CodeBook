#define GEN_PRINT(NAME, TYPE, SPECIFIER_STR)  \
    int print_generated_##NAME(TYPE x) {      \
        return printf(SPECIFIER_STR "\n", x); \
    }

GEN_PRINT(int, int, "%d")
GEN_PRINT(char, char, "%c")
GEN_PRINT(float, float, "%f")
GEN_PRINT(str, char*, "%s")
GEN_PRINT(ptr, void*, "%p")

#define print_any(X)                            \
    _Generic((X),                               \
            int : print_generated_int,          \
            float : print_generated_float,      \
            char : print_generated_char,        \
            char* : print_generated_str,        \
            default : print_generated_ptr       \
            )(X)

#ifdef DEBUG
    #define debug(X)            \
        do {                    \
            printf("%s: ", #X); \
            print_any(X);       \
        } while (0)
#else
    #define debug(X) 0
#endif

