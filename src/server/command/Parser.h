#pragma once

#include <vector>
#include <string>

#include "Command.h"

class Parser{
public:
    static CommandPtr parse(const std::string& input);
private:
    static std::vector<std::string> tokenize(const std::string& input);
    static CommandPtr create_command(const std::vector<std::string>& tokens);
};