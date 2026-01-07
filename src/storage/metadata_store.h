#pragma once
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

struct ObjectMeta {
    std::string key;
    std::size_t size{};
};

class MetadataStore {
public:
    void Put(const std::string& key, std::size_t size);
    std::optional<ObjectMeta> Get(const std::string& key) const;
    bool Erase(const std::string& key);

private:
    mutable std::mutex mu_;
    std::unordered_map<std::string, ObjectMeta> metas_;
};
