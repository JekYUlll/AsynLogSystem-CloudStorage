#include "common/log_sink.h"
#include <filesystem>

void LogSink::Open(const std::filesystem::path& dir) {
    dir_ = dir;
    std::filesystem::create_directories(dir_);
    out_.open(dir_ / "app.log", std::ios::app);
}

void LogSink::Write(const std::string& line) {
    if (out_) out_ << line;
}

void LogSink::Flush() {
    if (out_) out_.flush();
}
