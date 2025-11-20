 #pragma once
 #include "../Command.h"
 #include "../../database/Database.h"
 #include <string>

 class CountCommand : public Command {
public:
    CountCommand() = default;
    std::string execute(Database& db) override {
        return db.count();
    }
};