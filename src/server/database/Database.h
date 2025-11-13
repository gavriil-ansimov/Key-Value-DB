#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

class Database{
    std::unordered_map<std::string, std::string> db_;
public:
    std::string get(const std::string&);
    int count();
    void dump(const std::string&);

    void put(const std::string& key, const std::string& value);
    void del(const std::string& key);
    void load(const std::string& filename);

    std::string execute_command(std::string& cmd);
};


