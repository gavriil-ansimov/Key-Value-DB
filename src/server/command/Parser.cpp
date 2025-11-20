#include "Parser.h"
#include "commands/GetCommand.h"
#include "commands/PutCommand.h"
#include "commands/CountCommand.h"
#include "commands/DelCommand.h"
#include "commands/DumpCommand.h"
#include "commands/LoadCommand.h"

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
    else if (command == "PUT" && tokens.size() == 3) {
        return std::make_unique<PutCommand>(tokens[1], tokens[2]);
    }
    else if (command == "COUNT" && tokens.size() == 1) {
        return std::make_unique<CountCommand>();
    }
    else if (command == "DEL" && tokens.size() == 2) {
        return std::make_unique<DelCommand>(tokens[1]);
    }
    else if (command == "DUMP" && tokens.size() == 2) {
        return std::make_unique<DumpCommand>(tokens[1]);
    }
    else if (command == "LOAD" && tokens.size() == 2) {
        return std::make_unique<LoadCommand>(tokens[1]);
    }
    // ...
    return nullptr;
}