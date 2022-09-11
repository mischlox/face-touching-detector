#ifndef FT_DETECTOR_UTILS_H
#define FT_DETECTOR_UTILS_H
#include <cassert>
#include <chrono>

using namespace std::chrono_literals;

/**
 * Utility Class to measure time
 */
class Timer {
    std::chrono::steady_clock::time_point _start = std::chrono::steady_clock::now(), _end = {};

   public:
    void tick() {
        _end = std::chrono::steady_clock::time_point{};
        _start = std::chrono::steady_clock::now();
    }

    void tock() { _end = std::chrono::steady_clock::now(); }

    long elapsedTime() {
        assert(_end != std::chrono::steady_clock::time_point{} && "toc before reporting");
        return std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();
    }
};

#endif  // FT_DETECTOR_UTILS_H
