#include "Database.h"

std::string Database::get(const std::string& key) const {
    auto it = db_.find(key);
    
    if (it == db_.end())
        return "NE";

    return it->second;
}

std::string Database::count() const {
    return std::to_string(db_.size());
}

std::string Database::dump(const std::string& filename = "db.txt") const {
    std::ofstream out(filename);

    if (!out) throw std::runtime_error("Cannot open file");

    for (const auto& [key, value] : db_) {
        out << key << '\t' << value << std::endl;
    }

    return "OK";
}

std::string Database::put(const std::string& key, const std::string& value) {
    db_[key] = value;
    return "OK";
}

std::string Database::del(const std::string& key) {
    auto it = db_.find(key);
    
    if (it == db_.end())
        return "NE";

    db_.erase(key);
    return "OK";
}

std::string Database::load(const std::string& filename = "db.txt") {
    std::ifstream in(filename);

    if (!in) throw std::runtime_error("Cannot open file");
    std::string key, value;
    while(in >> key >> value) {
        db_[key] = value;
    }
    return "OK";
}