#include "Database.h"

std::string Database::get(const std::string& key) {
    auto it = db_.find(key);
    
    if (it == db_.end())
        return "No such key.";

    return it->second;
}
int Database::count() {
    return db_.size();
}
void Database::dump(const std::string& filename) {
    std::ofstream out(filename);

    if (!out) throw std::runtime_error("Cannot open file");

    for (const auto& [key, value] : db_) {
        out << key << '\t' << value << std::endl;
    }
}
void Database::put(const std::string& key, const std::string& value) {
    db_[key] = value;
}
void Database::del(const std::string& key) {
    db_.erase(key);
}
void Database::load(const std::string& filename) {
    std::ifstream in(filename);

    if (!in) throw std::runtime_error("Cannot open file");
    std::string key, value;
    while(in >> key >> value) {
        db_[key] = value;
    }
}