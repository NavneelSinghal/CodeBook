#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
using std::hardware_destructive_interference_size;
#else
// 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │
constexpr std::size_t hardware_constructive_interference_size = 64;
constexpr std::size_t hardware_destructive_interference_size = 64;
#endif

template <class T>
struct BinarySearcher {
    static constexpr std::size_t cache_line_size =
        hardware_destructive_interference_size;
    static constexpr std::size_t block_size = cache_line_size / sizeof(T);
    static constexpr std::size_t alignment =
        std::max(cache_line_size, alignof(T));
    T* b;
    std::vector<int> index;
    int n;
    BinarySearcher(const std::vector<T>& a)
        : b(static_cast<T*>(std::aligned_alloc(
              alignment, (sizeof(T) * (a.size() + 1) + alignment - 1) /
                             alignment * alignment))),
          index(a.size()),
          n((int)a.size()) {
        auto rearrange = [&](auto&& self, int a_index, int b_index) -> int {
            if (b_index <= n) {
                a_index = self(self, a_index, 2 * b_index);
                index[b_index] = a_index;
                b[b_index] = a[a_index++];
                a_index = self(self, a_index, 2 * b_index + 1);
            }
            return a_index;
        };
        index[0] = n;
        rearrange(rearrange, 0, 1);
    }
    ~BinarySearcher() { std::free(b); }
    template <class F>
    int find_first_false(F&& f) {
        int i = 1;
        while (i <= n) {
            if constexpr (block_size != 0)
                __builtin_prefetch(b + i * block_size);
            i = 2 * i + f(b[i]);
        }
        i >>= __builtin_ffs(~i);
        return index[i];
    }
};

/* sample usage
void test1() {
    Random rng;
    std::vector<int> a(n);
    std::generate(std::begin(a), std::end(a), [&] { return rng(1, 2 * n); });
    std::sort(std::begin(a), std::end(a));
    ll ans = 0;
    Stopwatch timer;
    BinarySearcher bs(a);
    for (int i = 0; i < n; ++i) {
        int q = rng(1, 2 * n);
        ans ^= bs.find_first_false([&](int cur) { return cur < q; });
    }
    std::cout << ans << '\n';
}
*/
