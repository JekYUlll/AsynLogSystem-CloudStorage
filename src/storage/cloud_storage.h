#pragma once
#include <filesystem>
#include <string>
#include "common/async_logger.h"

class CloudStorage {
public:
    explicit CloudStorage(AsyncLogger& logger, std::filesystem::path root = "data");
    bool PutObject(const std::string& key, const std::string& data);
    std::string GetObject(const std::string& key);
    bool DeleteObject(const std::string& key);

private:
    std::filesystem::path root_;
    AsyncLogger& logger_;
};
