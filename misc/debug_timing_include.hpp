#ifdef DEBUG
    #include "debug/debug.hpp"
#else
    #define debug(...) 0
#endif
#ifdef TIMING
    #include "debug/timing.hpp"
#else
// clang-format off
struct Stopwatch { Stopwatch(const string &s = "") {} void reset(){}
    void pause(){} void play(){} void print(){} ll elapsed() { return 0; }; };
// clang-format on
#endif
