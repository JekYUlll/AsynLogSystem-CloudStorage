#pragma once
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

enum class LogLevel { kDebug, kInfo, kWarn, kError };

constexpr inline const char* ToString(LogLevel level) {
    switch (level) {
        case LogLevel::kDebug: return "DEBUG";
        case LogLevel::kInfo:  return "INFO";
        case LogLevel::kWarn:  return "WARN";
        case LogLevel::kError: return "ERROR";
        default:              return "UNKNOWN";
    }
}

class AsyncLogger {
public:
    AsyncLogger();
    ~AsyncLogger();
    void Init(const std::filesystem::path& dir, size_t maxBytes = 4 * 1024 * 1024, int maxFiles = 5);
    void Log(LogLevel level, const std::string& msg);
    void Flush();

private:
    void Worker();
    std::string Format(LogLevel level, const std::string& msg);

    std::filesystem::path dir_;
    size_t max_bytes_{};
    int max_files_{};
    std::ofstream out_;
    std::mutex mu_;
    std::condition_variable cv_;
    std::queue<std::string> queue_;
    std::thread worker_;
    std::atomic<bool> stop_{false};
};
