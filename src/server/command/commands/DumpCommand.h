 #pragma once
 #include "../Command.h"
 #include <string>

 class DumpCommand : public Command {
public:
    DumpCommand(const std::string& name);
    std::string execute(Database& db) override;
    bool validate() const override;
private:
    std::string name_;
};