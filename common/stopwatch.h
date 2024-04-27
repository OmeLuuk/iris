#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <iostream>
#include <mutex>
#include <atomic>

class Stopwatch
{
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = Clock::duration;

    struct TimingData
    {
        Duration duration = Duration::zero();
        long long count = 0;
    };

    static int start(const std::string &key);
    static void stop(const std::string &key, int sessionId);
    static void report();

private:
    static std::unordered_map<std::string, std::unordered_map<int, TimePoint>> sessions;
    static std::unordered_map<std::string, TimingData> timings;
    static std::atomic<int> lastSessionId;
    static std::mutex mutex;
};
