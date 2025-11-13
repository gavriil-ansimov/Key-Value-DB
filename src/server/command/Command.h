#pragma once

#include <string>
#include <memory>

class Database;

class Command{
public:
    virtual ~Command()=default;

    virtual std::string execute(Database& db)=0;
    virtual bool validate() const=0;
};

using CommandPtr = std::unique_ptr<Command>;