 #pragma once
 #include "../Command.h"
 #include <string>

 class PutCommand : public Command {
public:
    PutCommand(const std::string& key, const std::string& value);
    std::string execute(Database& db) override;
    bool validate() const override;
private:
    std::string key_;
    std::string value_;
};