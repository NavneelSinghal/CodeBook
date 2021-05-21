template <typename T>
struct Range {
    struct It {
        T i;
        const T skip;
        void operator++() { i += skip; }
        T operator*() const { return i; }
        bool operator!=(It o) const {
            return (skip >= 0) ? (i < *o) : (i > *o);
        }
    };
    const T l_, r_, skip_;
    Range(T l, T r, T skip = 1) : l_(l), r_(r), skip_(skip) {
#ifdef DEBUG
        assert(skip != 0);
#endif
    }
    Range(T n) : Range(T(0), n, T(1)) {}
    It begin() const {
        return (skip_ >= 0) ? It{l_, skip_} : It{r_ - 1, skip_};
    }
    It end() const { return (skip_ >= 0) ? It{r_, skip_} : It{l_ - 1, skip_}; }
};

