namespace fft {

class cmplx {
   public:
    double a, b;
    cmplx() { a = 0.0, b = 0.0; }
    cmplx(double na, double nb = 0.0) { a = na, b = nb; }
    const cmplx operator+(const cmplx& c) { return cmplx(a + c.a, b + c.b); }
    const cmplx operator-(const cmplx& c) { return cmplx(a - c.a, b - c.b); }
    const cmplx operator*(const cmplx& c) {
        return cmplx(a * c.a - b * c.b, a * c.b + b * c.a);
    }
    double magnitude() { return sqrt(a * a + b * b); }
    void print() { cout << "(" << a << ", " << b << ")\n"; }
};

const double PI = acos(-1);

class fft {
   public:
    vector<cmplx> data, roots;
    vector<int32_t> rev;
    int32_t n, s;

    void setSize(int32_t ns) {
        s = ns;
        n = (1 << s);
        int32_t i, j;
        rev = vector<int32_t>(n);
        data = vector<cmplx>(n);
        roots = vector<cmplx>(n + 1);
        for (i = 0; i < n; ++i) {
            for (j = 0; j < s; ++j) {
                if (i & (1 << j)) {
                    rev[i] += (1 << (s - j - 1));
                }
            }
        }
        roots[0] = cmplx(1);
        cmplx mult = cmplx(cos(2 * PI / n), sin(2 * PI / n));
        for (i = 1; i <= n; ++i) {
            roots[i] = roots[i - 1] * mult;
        }
    }

    void bitReverse(vector<cmplx>& arr) {
        vector<cmplx> temp(n);
        int32_t i;
        for (i = 0; i < n; ++i) temp[i] = arr[rev[i]];
        for (i = 0; i < n; ++i) arr[i] = temp[i];
    }

    void transform(bool inverse = false) {
        bitReverse(data);
        int32_t i, j, k;
        for (i = 1; i <= s; ++i) {
            int32_t m = (1 << i), md2 = m >> 1;
            int32_t start = 0, increment = (1 << (s - i));
            if (inverse) {
                start = n;
                increment *= -1;
            }
            cmplx t, u;
            for (k = 0; k < n; k += m) {
                int32_t index = start;
                for (j = k; j < md2 + k; ++j) {
                    t = roots[index] * data[j + md2];
                    index += increment;
                    data[j + md2] = data[j] - t;
                    data[j] = data[j] + t;
                }
            }
        }
        if (inverse) {
            for (int32_t i = 0; i < n; ++i) {
                data[i].a /= n;
                data[i].b /= n;
            }
        }
    }

    static vector<int32_t> convolution(vector<int32_t>& a, vector<int32_t>& b) {
        int32_t alen = a.size();
        int32_t blen = b.size();
        int32_t resn = alen + blen - 1;
        int32_t s = 0, i;
        while ((1 << s) < resn) ++s;
        int32_t n = 1 << s;

        fft pga, pgb;
        pga.setSize(s);
        for (i = 0; i < alen; ++i) pga.data[i] = cmplx(a[i]);
        for (i = alen; i < n; ++i) pga.data[i] = cmplx(0);
        pga.transform();

        pgb.setSize(s);
        for (i = 0; i < blen; ++i) pgb.data[i] = cmplx(b[i]);
        for (i = blen; i < n; ++i) pgb.data[i] = cmplx(0);
        pgb.transform();

        for (i = 0; i < n; ++i) pga.data[i] = pga.data[i] * pgb.data[i];
        pga.transform(true);
        vector<int32_t> result(resn);
        for (i = 0; i < resn; ++i) result[i] = (int32_t)(pga.data[i].a + 0.5);

        int32_t actualSize = resn - 1;
        while (~actualSize && result[actualSize] == 0) --actualSize;
        if (actualSize < 0) actualSize = 0;
        result.resize(actualSize + 1);
        return result;
    }
};
}  // namespace fft
