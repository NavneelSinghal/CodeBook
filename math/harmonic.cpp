int64_t ans_clever = 0;
// [i..last] is the interval satisfying n / x == n / i <=> x \in [i..last]
for (int64_t i = 1, last; i <= n; i = last + 1) {
    last = n / (n / i);
    ans_clever += (last - i + 1) * (n / i);
}
