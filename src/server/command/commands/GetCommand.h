 #pragma once
 #include "../Command.h"
 #include <string>

 class GetCommand : public Command {
public:
    GetCommand(const std::string& key);
    std::string execute(Database& db) override;
    bool validate() const override;
private:
    std::string key_;
};