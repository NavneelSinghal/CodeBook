template <typename T = int>
T find_all_solutions(T a, T b, T c, T minx, T maxx, T miny, T maxy) {
    // stores result for sol to ax + by = g in x and y
    auto gcd = [](T a, T b, T& x, T& y) {
        x = 1, y = 0;
        T x1 = 0, y1 = 1, q;
        // invariant:
        // x * a_old + y * b_old = a
        // x_1 * a_old + y_1 * b_old = b
        while (b) {
            q = a / b;
            tie(x, x1) = make_pair(x1, x - q * x1);
            tie(y, y1) = make_pair(y1, y - q * y1);
            tie(a, b) = make_pair(b, a - q * b);
        }
        return a;
    };
    // solves ax + by = c and stores results in x, y - x, y are still changed if
    // it returns false
    auto find_any_sol = [](T a, T b, T c, T& x, T& y, T& g) {
        g = gcd(abs(a), abs(b), x, y);
        if (c % g) return false;
        x *= c / g;
        y *= c / g;
        if (a < 0) x = -x;
        if (b < 0) y = -y;
        return true;
    };

    auto shift_solution = [](T& x, T& y, T a, T b, T cnt) {
        x += cnt * b;
        y -= cnt * a;
    };

    T x, y, g;
    if (!find_any_solution(a, b, c, x, y, g)) return 0;
    a /= g;
    b /= g;

    int sign_a = a > 0 ? +1 : -1;
    int sign_b = b > 0 ? +1 : -1;

    shift_solution(x, y, a, b, (minx - x) / b);
    if (x < minx) shift_solution(x, y, a, b, sign_b);
    if (x > maxx) return 0;
    T lx1 = x;

    shift_solution(x, y, a, b, (maxx - x) / b);
    if (x > maxx) shift_solution(x, y, a, b, -sign_b);
    T rx1 = x;

    shift_solution(x, y, a, b, -(miny - y) / a);
    if (y < miny) shift_solution(x, y, a, b, -sign_a);
    if (y > maxy) return 0;
    T lx2 = x;

    shift_solution(x, y, a, b, -(maxy - y) / a);
    if (y > maxy) shift_solution(x, y, a, b, sign_a);
    T rx2 = x;

    if (lx2 > rx2) swap(lx2, rx2);
    T lx = max(lx1, lx2);
    T rx = min(rx1, rx2);

    // lx is the first value of x in the final interval

    if (lx > rx) return 0;
    return (rx - lx) / abs(b) + 1;
}

