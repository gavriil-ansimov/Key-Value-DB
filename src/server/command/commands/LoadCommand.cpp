#include "LoadCommand.h"
#include "../../database/Database.h"

LoadCommand::LoadCommand(const std::string& name)
    : name_(name) {}

std::string LoadCommand::execute(Database& db) {
    return db.load(name_);
}

bool LoadCommand::validate() const {
    std::regex pattern(R"(^[^/]+$)");

    return !name_.empty() && std::regex_match(name_, pattern);
}