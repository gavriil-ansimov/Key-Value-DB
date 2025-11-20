 #pragma once
 #include "../Command.h"
 #include <string>

 class DelCommand : public Command {
public:
    DelCommand(const std::string& key);
    std::string execute(Database& db) override;
    bool validate() const override;
private:
    std::string key_;
};