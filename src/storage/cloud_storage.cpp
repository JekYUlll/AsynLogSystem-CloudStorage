#include "storage/cloud_storage.h"
#include <fstream>

CloudStorage::CloudStorage(AsyncLogger& logger, std::filesystem::path root)
    : root_(std::move(root)), logger_(logger) {
    std::filesystem::create_directories(root_);
}

bool CloudStorage::PutObject(const std::string& key, const std::string& data) {
    auto path = root_ / key;
    std::ofstream ofs(path, std::ios::binary);
    if (!ofs) return false;
    ofs << data;
    logger_.Log(LogLevel::kInfo, "Put " + key);
    return true;
}

std::string CloudStorage::GetObject(const std::string& key) {
    auto path = root_ / key;
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) return {};
    std::string data((std::istreambuf_iterator<char>(ifs)), {});
    logger_.Log(LogLevel::kInfo, "Get " + key);
    return data;
}

bool CloudStorage::DeleteObject(const std::string& key) {
    auto path = root_ / key;
    auto ok = std::filesystem::remove(path);
    if (ok) logger_.Log(LogLevel::kInfo, "Delete " + key);
    return ok;
}
