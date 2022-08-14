void swap_detail(void* p1, void* p2, void* tmp, size_t pSize) {
    memcpy(tmp, p1, pSize), memcpy(p1, p2, pSize), memcpy(p2, tmp, pSize);
}
#define SWAP(a, b)                                                       \
    swap_detail(                                                         \
        &(a), &(b),                                                      \
        (char[(sizeof(a) == sizeof(b)) ? (ptrdiff_t)sizeof(a) : -1]){0}, \
        sizeof(a))


