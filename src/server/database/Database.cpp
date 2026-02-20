#include "Database.h"

#include "../logging/ILogger.h"

Database::Database(ILogger& logger)
    : logger_(logger) {}

std::string Database::get(const std::string& key) const {
    std::lock_guard<std::mutex> lock(m_);
    auto it = db_.find(key);
    
    if (it == db_.end()) {
        logger_.log(LogLevel::Warning, "Key not found");
        return "NE";
    }
    logger_.log(LogLevel::Debug, "GET " + key);
    return it->second;
}

std::string Database::count() const {
    std::lock_guard<std::mutex> lock(m_);

    logger_.log(LogLevel::Debug, "COUNT " + db_.size());
    return std::to_string(db_.size());
}

std::string Database::dump(const std::string& filename = "db.txt") const {
    
    std::unordered_map<std::string, std::string> snapshot;
    {
    std::lock_guard<std::mutex> lock(m_);
    snapshot = db_;
    }

    std::ofstream out("data/" + filename);

    if (!out) throw std::runtime_error("Cannot open file");

    for (const auto& [key, value] : snapshot) {
        out << key << '\t' << value << '\n';
    }
    logger_.log(LogLevel::Info, "Database dumped to " + filename);
    return "OK";
}

std::string Database::put(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(m_);
    db_[key] = value;
    logger_.log(LogLevel::Debug, "PUT " + key + ' ' + value);
    return "OK";
}

std::string Database::del(const std::string& key) {
    std::lock_guard<std::mutex> lock(m_);
    auto it = db_.find(key);
    
    if (it == db_.end()) {
        logger_.log(LogLevel::Warning, "Key not found");
        return "NE";
    }
    db_.erase(key);
    logger_.log(LogLevel::Debug, "DEL " + key);
    return "OK";
}

std::string Database::load(const std::string& filename = "db.txt") {
    std::lock_guard<std::mutex> lock(m_);

    std::ifstream in("data/" + filename);

    if (!in) throw std::runtime_error("Cannot open file");

    db_.clear();

    std::string key, value;
    while(in >> key >> value) {
        db_[key] = value;
    }
    logger_.log(LogLevel::Info, "Database loaded from " + filename);
    return "OK";
}