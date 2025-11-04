#pragma once

#include <string>
#include <vector>

#include "Database.h"

struct Parser{
    struct Command {
        std::string name;
        std::vector<std::string> args;
    };
    std::string parse(const std::string&, Database& db);
};
