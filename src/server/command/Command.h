#pragma once

#include <string>
#include <memory>
#include <regex>

class Database;

class Command{
public:
    virtual ~Command()=default;

    virtual std::string execute(Database& db)=0;
    virtual bool validate() const { return true; }
};

using CommandPtr = std::unique_ptr<Command>;