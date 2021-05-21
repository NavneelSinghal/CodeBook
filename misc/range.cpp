template <typename T>
struct iterable;
template <typename T>
struct iterable<T &> {
    using type = T &;
};
template <typename T>
struct iterable<T &&> {
    using type = T;
};
template <typename T, std::size_t N>
struct iterable<T(&&)[N]> {
    using type = typename T::rvalue_reference_to_array_is_not_supported;
};
template <typename T>
struct iterator_from_iterable {
    using iterable = typename std::remove_reference<T>::type &;
    using type = decltype(std::begin(std::declval<iterable>()));
};
template <typename T>
struct iterable_traits {
    using raw_iterable = T;
    using raw_iterator = typename iterator_from_iterable<raw_iterable>::type;
    using wrapped_iterable = typename iterable<T>::type;
    using deref_value_type = decltype(*std::declval<raw_iterator>());
};

template <typename T>
struct Range {
   private:
    const T l, r, skip;

   public:
    struct It : public std::iterator<std::forward_iterator_tag, T> {
        T i;
        const T skip;
        It(T _i, T _skip) : i(_i), skip(_skip) {}
        It &operator++() {
            i += skip;
            return *this;
        }
        const It &operator++(int) {
            auto temp = *this;
            operator++();
            return temp;
        }
        T operator*() const { return i; }
        bool operator!=(const It &o) const {
            return (skip >= 0) ? (i < *o) : (i > *o);
        }
        bool operator==(const It &o) const {
            return (skip >= 0) ? (i >= *o) : (i <= *o);
        }
    };
    using iterator = It;
    using value_type = T;
    Range(T _l, T _r, T _skip = 1) : l(_l), r(_r), skip(_skip) {
#ifdef DEBUG
        assert(skip_ != 0);
#endif
    }
    Range(T n) : Range(T(0), n, T(1)) {}
    iterator begin() const { return It{l, skip}; }
    iterator end() const { return It{r, skip}; }
    iterator begin() { return It{l, skip}; }
    iterator end() { return It{r, skip}; }
};

template <typename... T>
struct zip {
   public:
    using value_type =
        std::tuple<typename iterable_traits<T>::deref_value_type...>;
    using wrapped_iterables =
        std::tuple<typename iterable_traits<T>::wrapped_iterable...>;
    using raw_iterators =
        std::tuple<typename iterable_traits<T>::raw_iterator...>;
    using sequence = std::index_sequence_for<T...>;
    struct It : public std::iterator<std::forward_iterator_tag, value_type> {
       public:
        explicit It(raw_iterators iterators)
            : iterators_(std::move(iterators)) {}
        bool operator==(const It &it) const { return any_eq(it, sequence()); }
        bool operator!=(const It &it) const { return !any_eq(it, sequence()); }
        value_type operator*() const { return deref(sequence()); }
        It &operator++() { return inc(sequence()), *this; }
        const It &operator++(int) {
            auto temp = *this;
            return operator++(), temp;
        }

       private:
        raw_iterators iterators_;
        template <std::size_t... I>
        bool any_eq(const It &it, std::index_sequence<I...>) const {
            return (... ||
                    (std::get<I>(iterators_) == std::get<I>(it.iterators_)));
        }
        template <std::size_t... I>
        value_type deref(std::index_sequence<I...>) const {
            return {(*std::get<I>(iterators_))...};
        }
        template <std::size_t... I>
        void inc(std::index_sequence<I...>) {
            (++std::get<I>(iterators_), ...);
        }
    };
    using iterator = It;
    explicit zip(T &&... iterables)
        : iterables_(std::forward<T>(iterables)...) {}
    iterator begin() { return begin_(sequence()); }
    iterator end() { return end_(sequence()); }
    iterator begin() const { return begin_(sequence()); }
    iterator end() const { return end_(sequence()); }

   private:
    wrapped_iterables iterables_;
    template <std::size_t... Int>
    iterator begin_(std::index_sequence<Int...>) {
        return iterator(std::tuple(std::begin(std::get<Int>(iterables_))...));
    }
    template <std::size_t... Int>
    iterator end_(std::index_sequence<Int...>) {
        return iterator(std::tuple(std::end(std::get<Int>(iterables_))...));
    }
};

template <typename... T>
zip(T &&...) -> zip<T &&...>;

template <typename I>
struct enumerate {
   public:
    using size_type = typename std::make_signed<std::size_t>::type;
    using traits = iterable_traits<I>;
    using wrapped_iterable = typename traits::wrapped_iterable;
    using raw_iterator = typename traits::raw_iterator;
    using value_type = std::pair<size_type, typename traits::deref_value_type>;
    struct It : public std::iterator<std::forward_iterator_tag, value_type> {
        raw_iterator iter_;
        size_type start_;

       public:
        It(raw_iterator it, size_type start) : iter_(it), start_(start) {}
        bool operator==(const It &it) const { return iter_ == it.iter_; }
        bool operator!=(const It &it) const { return iter_ != it.iter_; }
        It &operator++() { return ++iter_, ++start_, *this; }
        const It operator++(int) {
            auto temp = *this;
            return operator++(), temp;
        }
        value_type operator*() const { return {start_, *iter_}; }
    };
    using iterator = It;
    explicit enumerate(I &&iterable, size_type start = 0)
        : iterable_(std::forward<I>(iterable)), start_(start) {}
    iterator begin() { return iterator(std::begin(iterable_), start_); }
    iterator end() { return iterator(std::end(iterable_), 0); }
    iterator begin() const { return iterator(std::begin(iterable_), start_); }
    iterator end() const { return iterator(std::end(iterable_), 0); }

   private:
    wrapped_iterable iterable_;
    size_type start_;
};

template <typename I>
enumerate(I &&) -> enumerate<I &&>;
template <typename I, typename Index>
enumerate(I &&, Index) -> enumerate<I &&>;

