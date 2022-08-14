#define IBUFSIZE /* */
#define OBUFSIZE /* */

char ibuf[IBUFSIZE], obuf[OBUFSIZE];
char *ibufptr = &*ibuf, *obufptr = &*obuf;

// assumes "x y z"
int read_non_negative() {
    ++ibufptr;
    int ans = 0;
    while (*ibufptr >= '0') ans = ans * 10 + *(ibufptr++) - '0';
    return ans;
}

int read_int() {
    ++ibufptr;
    int ans = 0;
    int sign = 1;
    if (*ibufptr == '-') sign = -1, ibufptr++;
    while (*ibufptr >= '0') ans = ans * 10 + *(ibufptr++) - '0';
    return ans * sign;
}

void print_char(char x) {
    *(obufptr++) = x;
}

// 1 2 3 4 5 6 7 8 9 10
void print_non_negative(int x) {
#define PRINT_MID(X, P) print_char((x / P) % 10 + 48)
#define PRINT_TOP(X, P) print_char(x / P + 48)
#define PRINT_BOTTOM(X) print_char(x % 10 + 48)
    if (x < 1000000) {
        if (x < 1000) {
            if (x < 10) {
                print_char(x + 48);
            } else if (x < 100) {
                PRINT_TOP(X, 10);
                PRINT_BOTTOM(X);
            } else {
                PRINT_TOP(X, 100);
                PRINT_MID(X, 10);
                PRINT_BOTTOM(X);
            }
        } else {
            if (x < 10000) {
                PRINT_TOP(X, 1000);
                PRINT_MID(X, 100);
                PRINT_MID(X, 10);
                PRINT_BOTTOM(X);
            } else if (x < 100000) {
                PRINT_TOP(X, 10000);
                PRINT_MID(X, 1000);
                PRINT_MID(X, 100);
                PRINT_MID(X, 10);
                PRINT_BOTTOM(X);
            } else {
                PRINT_TOP(X, 100000);
                PRINT_MID(X, 10000);
                PRINT_MID(X, 1000);
                PRINT_MID(X, 100);
                PRINT_MID(X, 10);
                PRINT_BOTTOM(X);
            }
        }
    } else {
        if (x < 100000000) {
            if (x < 10000000) {
                PRINT_TOP(X, 1000000);
                PRINT_MID(X, 100000);
                PRINT_MID(X, 10000);
                PRINT_MID(X, 1000);
                PRINT_MID(X, 100);
                PRINT_MID(X, 10);
                PRINT_BOTTOM(X);
            } else {
                PRINT_TOP(X, 10000000);
                PRINT_MID(X, 1000000);
                PRINT_MID(X, 100000);
                PRINT_MID(X, 10000);
                PRINT_MID(X, 1000);
                PRINT_MID(X, 100);
                PRINT_MID(X, 10);
                PRINT_BOTTOM(X);
            }
        } else {
            if (x < 1000000000) {
                PRINT_TOP(X, 100000000);
                PRINT_MID(X, 10000000);
                PRINT_MID(X, 1000000);
                PRINT_MID(X, 100000);
                PRINT_MID(X, 10000);
                PRINT_MID(X, 1000);
                PRINT_MID(X, 100);
                PRINT_MID(X, 10);
                PRINT_BOTTOM(X);
            } else {
                PRINT_TOP(X, 1000000000);
                PRINT_MID(X, 100000000);
                PRINT_MID(X, 10000000);
                PRINT_MID(X, 1000000);
                PRINT_MID(X, 100000);
                PRINT_MID(X, 10000);
                PRINT_MID(X, 1000);
                PRINT_MID(X, 100);
                PRINT_MID(X, 10);
                PRINT_BOTTOM(X);
            }
        }
    }
#undef PRINT_MID
#undef PRINT_TOP
#undef PRINT_BOTTOM
}

void print(int x) {
    if (x < 0)
        print_char('-'), print_non_negative(-x);
    else
        print_non_negative(x);
}

// at the beginning of main():
// fread(ibuf + 1, 1, sizeof(ibuf) - 1, stdin);

// at the end of main():
// fwrite(obuf, 1, obufptr - obuf, stdout);
