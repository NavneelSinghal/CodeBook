struct u128 {
    using u32 = uint32_t;
    using u64 = uint64_t;
    using i64 = int64_t;

    u64 high, low;

    u128() : high(0), low(0) {}
    u128(u64 x) : high(0), low(x) {}
    u128(u64 x, u64 y) : high(x), low(y) {}
    u128(const u128&) = default;
    u128(u128&&) = default;

    static u128 multiply(u64 x, u64 y) {
        u32 a = u32(x >> 32), b = u32(x);  // way of getting high and low bits
        u32 c = u32(y >> 32), d = u32(y);
        u64 ac = u64(a) * c;
        u64 ad = u64(a) * d;
        u64 bc = u64(b) * c;
        u64 bd = u64(b) * d;
        u64 carry = u64(u32(ad)) + u64(u32(bc)) + (bd >> 32);
        u64 high = ac + (ad >> 32) + (bc >> 32) + (carry >> 32);
        u64 low = (ad << 32) + (bc << 32) + bd;
        return u128{high, low};
    }

    bool operator==(const u128& other) const {
        return high == other.high && low == other.low;
    }

    // only works iff result is guaranteed to be 128 bit
    // alternatively, multiplication modulo 2^128
    static u128 multiply(u128 a, u64 b) {
        u128 ans = u128::multiply(a.low, b);
        ans.high += u128::multiply(a.high, b).low;
        return ans;
    }

    // only works iff result is guaranteed to be 128 bit
    // alternatively, multiplication modulo 2^128
    static u128 multiply(u64 a, u128 b) { return u128::multiply(b, a); }

    // only works iff result is guaranteed to be 128 bit
    // alternatively, multiplication modulo 2^128
    static u128 multiply(u128 a, u128 b) {
        u128 ans = u128::multiply(a, b.low);
        u128 ans2 = u128::multiply(a.low, b.high);
        return u128::add(ans, ans2);
    }

    // only works iff a >= b
    // alternatively, subtraction modulo 2^128
    static u128 sub(u128 a, u128 b) {
        a.high -= (a.low < b.low) + b.high;
        a.low -= b.low;
        return a;
    }

    // addition mod 2^128
    static u128 add(u128 a, u128 b) {
        u128 ans;
        ans.low = a.low + b.low;
        if (ans.low < a.low) a.high++;
        ans.high = a.high + b.high;
        return ans;
    }

    // adapted from
    // https://github.com/hcs0/Hackers-Delight/blob/master/divmnu.c.txt
    static std::pair<u128, u64> divide(u128 dividend, u64 divisor) {
        if (dividend == u128{0, 0}) return std::make_pair(dividend, 0);
        static constexpr u64 b = 1ULL << 32;

        /*
           v[N], r[N], u[M], q[M], vn[N], un[M + 1]
           N = sizeof(dividend) / 4
           M = sizeof(divisor) / 4

           to adapt this implementation, change stuff till (*)
           */
        u32 v[2], r[2], u[4], q[4], vn[2], un[5];

        auto compute = [&] {
            u128 quotient;
            u64 rem;
            rem = r[0] + r[1] * b;
            quotient.high = q[2] + q[3] * b;
            quotient.low = q[0] + q[1] * b;
            return std::make_pair(quotient, rem);
        };

        u[0] = u32(dividend.low);
        u[1] = u32(dividend.low >> 32);
        u[2] = u32(dividend.high);
        u[3] = u32(dividend.high >> 32);

        v[0] = u32(divisor);
        v[1] = u32(divisor >> 32);

        int n = 0, m = 0;
        for (int i = 3; i >= 0; --i) {
            if (u[i]) {
                m = i + 1;
                break;
            }
        }
        for (int i = 1; i >= 0; --i) {
            if (v[i]) {
                n = i + 1;
                break;
            }
        }

        if (m < n) return std::make_pair(u128(), dividend.low);
        
        // (*)

        u64 qhat{}, rhat{};
        u64 p{};
        int i{}, j{}, s{};

        if (n == 1) {
            u64 k = 0;
            auto d = v[0];
            for (j = m - 1; j >= 0; j--) {
                auto D = (k * b + u[j]);
                q[j] = u32(D / d);
                k = u64(D - u64(q[j]) * d);
            }
            r[0] = u32(k);
            return compute();
        }

        i64 t{}, k{};  // weird

        s = __builtin_clzll(v[n - 1]) - 32;

        // s != 32 since v[n - 1] is guaranteed to be nonzero
        if (s == 0) {
            for (i = n - 1; i >= 0; --i) vn[i] = v[i];
            un[m] = 0;
            for (i = m - 1; i >= 0; --i) un[i] = u[i];
        } else {
            for (i = n - 1; i > 0; --i)
                vn[i] = (v[i] << s) | (v[i - 1] >> (32 - s));
            vn[0] = v[0] << s;
            un[m] = u[m - 1] >> (32 - s);
            for (i = m - 1; i > 0; --i)
                un[i] = (u[i] << s) | (u[i - 1] >> (32 - s));
            un[0] = u[0] << s;
        }

        for (j = m - n; j >= 0; --j) {
            auto D = (un[j + n] * b + un[j + n - 1]);
            qhat = D / vn[n - 1];
            rhat = D - qhat * vn[n - 1];
            while (true) {
                if (qhat >= b || qhat * vn[n - 2] > b * rhat + un[j + n - 2]) {
                    qhat--;
                    rhat += vn[n - 1];
                    if (rhat < b) continue;
                }
                break;
            }

            k = 0;  // weird
            for (i = 0; i < n; i++) {
                p = qhat * vn[i];                       // weird
                t = un[i + j] - k - (p & 0xFFFFFFFFU);  // weird
                un[i + j] = u32(t);                     // weird
                k = (p >> 32) - (t >> 32);              // weird
            }
            t = un[j + n] - k;   // weird
            un[j + n] = u32(t);  // weird

            q[j] = u32(qhat);
            if (t < 0) {
                q[j]--;
                k = 0;  // weird
                for (i = 0; i < n; i++) {
                    t = un[i + j] + vn[i] + k;  // weird
                    un[i + j] = u32(t);         // weird
                    k = t >> 32;                // weird
                }
                un[j + n] = u32(un[j + n] + k);  // weird
            }
        }
        if (s == 0)
            for (i = 0; i < n; ++i) r[i] = un[i];
        else
            for (i = 0; i < n; i++)
                r[i] = (un[i] >> s) | (un[i + 1] << (32 - s));
        return compute();
    }

    static u64 mod(u128 a, u64 b) { return u128::divide(a, b).second; }
};


