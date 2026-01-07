#include "storage/metadata_store.h"

void MetadataStore::Put(const std::string& key, std::size_t size) {
    std::lock_guard<std::mutex> lk(mu_);
    metas_[key] = ObjectMeta{key, size};
}

std::optional<ObjectMeta> MetadataStore::Get(const std::string& key) const {
    std::lock_guard<std::mutex> lk(mu_);
    auto it = metas_.find(key);
    if (it == metas_.end()) return std::nullopt;
    return it->second;
}

bool MetadataStore::Erase(const std::string& key) {
    std::lock_guard<std::mutex> lk(mu_);
    return metas_.erase(key) > 0;
}
