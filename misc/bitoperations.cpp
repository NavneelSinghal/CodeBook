template <class T, bool unsafe = false>
constexpr uint32_t ctz(T a) {
    if constexpr (!unsafe) {
        if (!a) return sizeof(T) * 8;
    }
    if constexpr (sizeof(T) <= sizeof(uint32_t)) {
        return (uint32_t)(__builtin_ctz((uint32_t)a));
    } else if constexpr (sizeof(T) <= sizeof(uint64_t)) {
        return (uint32_t)(__builtin_ctzll((uint64_t)a));
    } else {
        static_assert(sizeof(T) == sizeof(uint64_t) * 2);
        uint32_t l = ctz((uint64_t)a);
        return l != sizeof(uint64_t) * 8
                   ? l
                   : l + ctz((uint64_t)(a >> sizeof(uint64_t) * 8));
    }
}

template <class T, bool unsafe = false>
constexpr uint32_t clz(T a) {
    if constexpr (!unsafe) {
        if (!a) return sizeof(T) * 8;
    }
    if constexpr (sizeof(T) <= sizeof(uint32_t)) {
        return (uint32_t)(__builtin_clz((uint32_t)a));
    } else if constexpr (sizeof(T) <= sizeof(uint64_t)) {
        return (uint32_t)(__builtin_clzll((uint64_t)a));
    } else {
        static_assert(sizeof(T) == sizeof(uint64_t) * 2);
        uint32_t l = clz((uint64_t)(a >> sizeof(uint64_t) * 8));
        return l != sizeof(uint64_t) * 8 ? l : l + clz((uint64_t)a);
    }
}

// -1 for 0
template <class T, bool unsafe = false>
constexpr uint32_t lg(T a) {
    if constexpr (sizeof(T) <= sizeof(uint32_t)) {
        return sizeof(uint32_t) * 8 - 1 - clz<T, unsafe>(a);
    } else if constexpr (sizeof(T) <= sizeof(uint64_t)) {
        return sizeof(uint64_t) * 8 - 1 - clz<T, unsafe>(a);
    } else {
        return sizeof(uint64_t) * 16 - 1 - clz<T, unsafe>(a);
    }
}

