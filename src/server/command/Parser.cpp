#include "Parser.h"
#include "commands/GetCommand.h"

#include <sstream>

CommandPtr Parser::parse(const std::string& input) {
    auto tokens = tokenize(input);
    if (tokens.empty())
        return nullptr;
    return create_command(tokens);
}

std::vector<std::string> Parser::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);

    std::string token;
    while(iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

CommandPtr Parser::create_command(const std::vector<std::string>& tokens) {
    const std::string& command = tokens[0];

    if (command == "GET" && tokens.size() == 2) {
        return std::make_unique<GetCommand>(tokens[1]);
    }
    // ...
    return nullptr;
}