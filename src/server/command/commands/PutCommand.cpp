#include "PutCommand.h"
#include "../../database/Database.h"

PutCommand::PutCommand(const std::string& key, const std::string& value)
    : key_(key), value_(value) {}

std::string PutCommand::execute(Database& db) {
    return db.put(key_, value_);
}

bool PutCommand::validate() const {
    std::regex pattern(R"(^[A-Za-z0-9]+$)");
    return !key_.empty() && std::regex_match(key_, pattern);
}