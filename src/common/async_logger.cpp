#include "common/async_logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>

AsyncLogger::AsyncLogger() = default;

AsyncLogger::~AsyncLogger() {
    stop_.store(true);
    cv_.notify_all();
    if (worker_.joinable()) worker_.join();
    Flush();
}

void AsyncLogger::Init(const std::filesystem::path& dir, size_t maxBytes, int maxFiles) {
    dir_ = dir;
    max_bytes_ = maxBytes;
    max_files_ = maxFiles;
    std::filesystem::create_directories(dir_);
    out_.open(dir_ / "app.log", std::ios::app);
    worker_ = std::thread(&AsyncLogger::Worker, this);
}

void AsyncLogger::Log(LogLevel level, const std::string& msg) {
    {
        std::lock_guard<std::mutex> lk(mu_);
        queue_.push(Format(level, msg));
    }
    cv_.notify_one();
}

void AsyncLogger::Flush() {
    std::lock_guard<std::mutex> lk(mu_);
    if (out_) out_.flush();
}

std::string AsyncLogger::Format(LogLevel level, const std::string& msg) {
    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    #ifdef _WIN32
    localtime_s(&tm, &tt);
    #else
    localtime_r(&tt, &tm);
    #endif
    std::ostringstream os;
    os << std::put_time(&tm, "%F %T") << " [" << ToString(level) << "] " << msg << '\n';
    return os.str();
}

void AsyncLogger::Worker() {
    std::unique_lock<std::mutex> lk(mu_);
    for (;;) {
        cv_.wait(lk, [&] { return stop_.load() || !queue_.empty(); });

        while (!queue_.empty()) {
            auto line = std::move(queue_.front());
            queue_.pop();
            lk.unlock();
            out_ << line;
            lk.lock();
        }
        out_.flush();

        if (stop_.load() && queue_.empty()) break; // ensure pending logs drained before exit
    }
}
