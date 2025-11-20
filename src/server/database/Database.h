#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

class Database{
    std::unordered_map<std::string, std::string> db_;
public:
    std::string get(const std::string&) const;
    std::string count() const;
    std::string dump(const std::string&) const;

    std::string put(const std::string& key, const std::string& value);
    std::string del(const std::string& key);
    std::string load(const std::string& filename);
};