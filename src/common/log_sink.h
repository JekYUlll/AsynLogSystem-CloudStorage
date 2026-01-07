#pragma once
#include <filesystem>
#include <fstream>
#include <string>

class LogSink {
public:
    void Open(const std::filesystem::path& dir);
    void Write(const std::string& line);
    void Flush();

private:
    std::filesystem::path dir_;
    std::ofstream out_;
};
