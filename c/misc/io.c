#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define IBUFSIZE /* TODO */
#define OBUFSIZE /* TODO */

char ibuf[IBUFSIZE], obuf[OBUFSIZE];
char *ibufptr = &*ibuf, *obufptr = &*obuf;

void print_char(char x) {
    *(obufptr++) = x;
}

void print_str(const char* const c, size_t len) {
    memcpy(obufptr, c, len);
    obufptr += len;
}

#define PRINT_UNSIGNED(X)            \
    void print_u##X(uint##X##_t x) { \
        if (x < 10)                  \
            print_char(x + 48);      \
        else {                       \
            print_u##X(x / 10);      \
            print_char(x % 10 + 48); \
        }                            \
    }

#define unlikely(x) __builtin_expect(!!(x), 0)
#define PRINT_SIGNED(X)                                        \
    void print_i##X(int##X##_t x) {                            \
        static char buf[100];                                  \
        if (unlikely(x == INT##X##_MIN)) {                     \
            int len = sprintf(buf, "%" PRIi##X, INT##X##_MIN); \
            print_str(buf, len);                               \
        } else if (x < 0) {                                    \
            print_char('-');                                   \
            print_u##X(-x);                                    \
        } else {                                               \
            print_u##X(x);                                     \
        }                                                      \
    }

PRINT_UNSIGNED(32);
PRINT_UNSIGNED(64);
PRINT_SIGNED(32);
PRINT_SIGNED(64);

void skip_char() {
    ++ibufptr;
}

// can do skip_spaces instead of skip_char for this purpose, but it will be
// slower

// assumes "x y z"
#define READ_UNSIGNED(X)                                             \
    uint##X##_t read_u##X() {                                        \
        skip_char();                                                 \
        uint##X##_t ans = 0;                                         \
        while (*ibufptr >= '0') ans = ans * 10 + *(ibufptr++) - '0'; \
        return ans;                                                  \
    }
#define READ_SIGNED(X)                                               \
    uint##X##_t read_i##X() {                                        \
        skip_char();                                                 \
        int##X##_t ans = 0, sign = 1;                                \
        if (*ibufptr == '-') sign = -1, ++ibufptr;                   \
        while (*ibufptr >= '0') ans = ans * 10 + *(ibufptr++) - '0'; \
        return sign * ans;                                           \
    }

READ_UNSIGNED(32);
READ_UNSIGNED(64);
READ_SIGNED(32);
READ_SIGNED(64);

void load_str(char* s) {
    skip_char();
    while (*ibufptr != ' ' && *ibufptr != '\n') *(s++) = *(ibufptr++);
}

// at the beginning of main():
// fread(ibuf + 1, 1, sizeof(ibuf) - 1, stdin);

// at the end of main():
// fwrite(obuf, 1, obufptr - obuf, stdout);
