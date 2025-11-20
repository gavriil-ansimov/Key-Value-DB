#include "DumpCommand.h"
#include "../../database/Database.h"

DumpCommand::DumpCommand(const std::string& name)
    : name_(name) {}

std::string DumpCommand::execute(Database& db) {
    return db.dump(name_);
}

bool DumpCommand::validate() const {
    std::regex pattern(R"(^[^/]+$)");

    return !name_.empty() && std::regex_match(name_, pattern);
}