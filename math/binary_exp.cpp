// basic binary exponentiation
// T can be any class with associative multiplication defined on it

template <class T, class F = multiplies<T>>
T pwr(T a, long long n, F op = multiplies<T>(), T e = {1}) {
    // assert(n >= 0);
    T res = e;
    while (n) {
        if (n & 1) res = op(res, a);
        if (n >>= 1) a = op(a, a);
    }
    return res;
}

// fibonacci

struct matrix {
    int a[2][2];
    matrix(int a = 1, int b = 0, int c = 0, int d = 1) {
        a[0][0] = a;
        a[0][1] = b;
        a[1][0] = c;
        a[1][1] = d;
    }
    matrix operator * (matrix a) {
        matrix r;
        r.a[0][0] = (1LL * a[0][0] * a.a[0][0] + 1LL * a[0][1] * a.a[1][0]) % mod;
        r.a[0][1] = (1LL * a[0][0] * a.a[0][1] + 1LL * a[0][1] * a.a[1][1]) % mod;
        r.a[1][0] = (1LL * a[1][0] * a.a[0][0] + 1LL * a[1][1] * a.a[1][0]) % mod;
        r.a[1][1] = (1LL * a[1][0] * a.a[0][1] + 1LL * a[1][1] * a.a[1][1]) % mod;
        return r;
    }
}
// pwr(matrix(1, 1, 1, 0), n, matrix(1, 0, 0, 1)).a[][]

// relevant contest for only binary exponentiation - https://codeforces.com/gym/102644

// when you're trying to find a^n mod p, with n very large and p prime, then you can do this to reduce n:
// if a is 0 mod p, return (n == 0)
// replace n by n mod (p - 1)
// do the usual exponentiation

// when trying to find x^n mod m, with n > __lg(m) (m can be int, long or long long) try to do this
// return x^(phi(m) + n mod phi(m)) mod m
