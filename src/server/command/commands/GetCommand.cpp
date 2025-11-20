#include "GetCommand.h"
#include "../../database/Database.h"

GetCommand::GetCommand(const std::string& key)
    : key_(key) {}

std::string GetCommand::execute(Database& db) {
    return db.get(key_);
}

bool GetCommand::validate() const {
    std::regex pattern(R"(^[A-Za-z0-9]+$)");
    return !key_.empty() && std::regex_match(key_, pattern);
}