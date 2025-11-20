#include "DelCommand.h"
#include "../../database/Database.h"

DelCommand::DelCommand(const std::string& key)
    : key_(key) {}

std::string DelCommand::execute(Database& db) {
    return db.del(key_);
}

bool DelCommand::validate() const {
    std::regex pattern(R"(^[A-Za-z0-9]+$)");
    return !key_.empty() && std::regex_match(key_, pattern);
}