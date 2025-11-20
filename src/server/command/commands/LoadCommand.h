 #pragma once
 #include "../Command.h"
 #include <string>

 class LoadCommand : public Command {
public:
    LoadCommand(const std::string& name);
    std::string execute(Database& db) override;
    bool validate() const override;
private:
    std::string name_;
};