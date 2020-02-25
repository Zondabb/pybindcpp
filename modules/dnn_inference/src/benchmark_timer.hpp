#ifndef __BENCHMARK_TIMER_HPP__
#define __BENCHMARK_TIMER_HPP__

#include <chrono>
#include <string>
#include "config.hpp"

enum class TimeUnit {
    milliseconds, microseconds
};

template<TimeUnit timeUnit = TimeUnit::milliseconds>
class BenchmarkTimer {
public:
    explicit BenchmarkTimer(std::string tag) : begin{
        std::chrono::steady_clock::now()}, mTag{std::move(tag)} {
    }

    explicit BenchmarkTimer() : 
        begin{std::chrono::steady_clock::now()}, 
        mTag{"Timer"} {}

    ~BenchmarkTimer() {
        if (!isStop) {
            stop();
        }
    }

    void stop() {
        stopImpl(std::integral_constant<TimeUnit, timeUnit>{});
        isStop = true;
    }

private:

    void stopImpl(std::integral_constant<TimeUnit, TimeUnit::milliseconds>) {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        INFO_LOG("[ %s ] cost time = %lld ms", mTag.c_str(),
                std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
    }

    void stopImpl(std::integral_constant<TimeUnit, TimeUnit::microseconds>) {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        INFO_LOG("[ %s ] cost time = %lld us", mTag.c_str(),
                std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
    }

private:
    std::chrono::steady_clock::time_point begin;
    std::string mTag;
    bool isStop = false;
};

#endif // __BENCHMARK_TIMER_HPP__