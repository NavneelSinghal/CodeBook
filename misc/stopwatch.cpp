struct Stopwatch {
    std::string name = "Time";
    std::chrono::time_point<std::chrono::steady_clock> t =
        std::chrono::steady_clock::now();
    void restart() { t = std::chrono::steady_clock::now(); }
    int elapsed() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - t)
            .count();
    }
    void print() const {
#ifdef TIMING
        std::cerr << name << ": " << elapsed() << " ms\n";
#endif
    }
    ~Stopwatch() { print(); }
};

