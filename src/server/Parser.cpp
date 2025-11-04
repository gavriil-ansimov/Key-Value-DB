#include "Parser.h"
#include <sstream>

std::string Parser::parse(const std::string& in, Database& db) {
    std::stringstream ss;
    Command cmd;

    ss << in;
    ss >> cmd.name;

    std::string arg;
    while (ss >> arg) {
        cmd.args.push_back(arg);
    }

    if (cmd.name == "PUT") {
        db.put(cmd.args[0], cmd.args[1]);
        return "OK";
    }
    else if (cmd.name == "GET") {
        return db.get(cmd.args[0]);
    }
    else if (cmd.name == "DEL") {
        db.del(cmd.args[0]);
        return "OK";
    }
    else if (cmd.name == "COUNT") {
        return std::to_string(db.count());
    }
    else if (cmd.name == "DUMP") {
        db.dump(cmd.args[0]);
        return "OK";
    }
    else {
        return "Unknown command";
    }
}