#include "stopwatch.h"
#include "logging.h"

std::unordered_map<std::string, std::unordered_map<int, Stopwatch::TimePoint>> Stopwatch::sessions;
std::unordered_map<std::string, Stopwatch::TimingData> Stopwatch::timings;
std::atomic<int> Stopwatch::lastSessionId{0};
std::mutex Stopwatch::mutex;

int Stopwatch::start(const std::string &key)
{
    std::lock_guard<std::mutex> lock(mutex);
    int sessionId = ++lastSessionId; // Generate a unique session ID
    sessions[key][sessionId] = Clock::now();
    return sessionId;
}

void Stopwatch::stop(const std::string &key, int sessionId)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto keyIt = sessions.find(key);
    if (keyIt != sessions.end())
    {
        auto &sessionMap = keyIt->second;
        auto sessionIt = sessionMap.find(sessionId);
        if (sessionIt != sessionMap.end())
        {
            Duration duration = Clock::now() - sessionIt->second;
            timings[key].duration += duration;
            timings[key].count++;
            sessionMap.erase(sessionIt); // Clean up the session
        }
    }
}

void Stopwatch::report()
{
    std::lock_guard<std::mutex> lock(mutex);
    for (const auto &pair : timings)
    {
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(pair.second.duration).count();
        std::cout << pair.first << ": Total Time = " << millis << " ms, "
                  << "Count = " << pair.second.count << ", "
                  << "Average = " << (pair.second.count ? millis / pair.second.count : 0) << " ms\n";
    }
}
