template <typename C = std::chrono::steady_clock,
          typename T1 = std::chrono::nanoseconds,
          typename T2 = std::chrono::milliseconds>
struct Stopwatch {
    std::string name;
    std::chrono::time_point<C> last_played;
    T1 elapsed_time;
    bool running;
    Stopwatch(const std::string &s) : name(s), running(true) { reset(); }
    Stopwatch() : Stopwatch("Time") {}
    void reset() {
        last_played = C::now();
        elapsed_time = T1::zero();
    }
    void pause() {
        if (!running) return;
        running = false;
        elapsed_time += C::now() - last_played;
    }
    void play() {
        if (running) return;
        running = true;
        last_played = C::now();
    }
    int_fast64_t elapsed() const {
        return std::chrono::duration_cast<T2>(
                   elapsed_time +
                   (running ? (C::now() - last_played) : T1::zero()))
            .count();
    }
    void print() const {
#ifdef TIMING
        std::cerr << name << ": " << elapsed() << " ms\n";
#endif
    }
    ~Stopwatch() { print(); }
};

