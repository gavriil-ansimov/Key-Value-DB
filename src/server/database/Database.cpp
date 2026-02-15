#include "Database.h"

std::string Database::get(const std::string& key) const {
    std::lock_guard<std::mutex> lock(m_);
    auto it = db_.find(key);
    
    if (it == db_.end())
        return "NE";

    return it->second;
}

std::string Database::count() const {
    std::lock_guard<std::mutex> lock(m_);
    return std::to_string(db_.size());
}

std::string Database::dump(const std::string& filename = "db.txt") const {
    
    std::unordered_map<std::string, std::string> snapshot;
    {
    std::lock_guard<std::mutex> lock(m_);
    snapshot = db_;
    }

    std::ofstream out(filename);

    if (!out) throw std::runtime_error("Cannot open file");

    for (const auto& [key, value] : snapshot) {
        out << key << '\t' << value << '\n';
    }

    return "OK";
}

std::string Database::put(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(m_);
    db_[key] = value;
    return "OK";
}

std::string Database::del(const std::string& key) {
    std::lock_guard<std::mutex> lock(m_);
    auto it = db_.find(key);
    
    if (it == db_.end())
        return "NE";

    db_.erase(key);
    return "OK";
}

std::string Database::load(const std::string& filename = "db.txt") {
    std::lock_guard<std::mutex> lock(m_);

    std::ifstream in(filename);

    if (!in) throw std::runtime_error("Cannot open file");

    db_.clear();

    std::string key, value;
    while(in >> key >> value) {
        db_[key] = value;
    }
    return "OK";
}