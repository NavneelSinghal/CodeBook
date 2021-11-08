template <class Integer, class F>
Integer find_first_false(Integer l, Integer r,
                         F&& f) requires(sizeof(Integer) == 8) {
    if (l > r) return r + 1;
    ++r;
    int p = __lg(r - l);
    Integer w = Integer(1) << p;
    --l;
    if (f(l + w)) l = r - w;
    switch (p) {
#define C(x) \
    case x:  \
        if (f(l + (1LL << (x - 1)))) l += (1LL << (x - 1));
        C(63)
        C(62)
        C(61) C(60) C(59) C(58) C(57) C(56) C(55) C(54) C(53) C(52) C(51) C(50)
            C(49) C(48) C(47) C(46) C(45) C(44) C(43) C(42) C(41) C(40) C(39)
                C(38) C(37) C(36) C(35) C(34) C(33) C(32) C(31) C(30) C(29)
                    C(28) C(27) C(26) C(25) C(24) C(23) C(22) C(21) C(20) C(19)
                        C(18) C(17) C(16) C(15) C(14) C(13) C(12) C(11) C(10)
                            C(9) C(8) C(7) C(6) C(5) C(4) C(3) C(2) C(1);
#undef C
        default:
            break;
    }
    return l + 1;
}
template <class Integer, class F>
Integer find_first_false(Integer l, Integer r,
                         F&& f) requires(sizeof(Integer) == 4) {
    if (l > r) return r + 1;
    ++r;
    int p = __lg(r - l);
    Integer w = Integer(1) << p;
    --l;
    if (f(l + w)) l = r - w;
    switch (p) {
#define C(x) \
    case x:  \
        if (f(l + (1 << (x - 1)))) l += (1 << (x - 1));
        C(31)
        C(30) C(29) C(28) C(27) C(26) C(25) C(24) C(23) C(22) C(21) C(20) C(19)
            C(18) C(17) C(16) C(15) C(14) C(13) C(12) C(11) C(10) C(9) C(8) C(7)
                C(6) C(5) C(4) C(3) C(2) C(1);
#undef C
        default:
            break;
    }
    return l + 1;
}
