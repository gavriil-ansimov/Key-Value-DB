#include "Session.h"
#include "../database/Database.h"
#include "../command/Parser.h"

#include <iostream>
#include <sstream>

Session::Session(tcp::socket socket, Database& db)
    : socket_(std::move(socket)), db_(db) {}

void Session::start() {
    do_read();
}

void Session::do_read() {
    auto self(shared_from_this());

    boost::asio::async_read_until(socket_, buffer_, '\n',
    [this, self](boost::system::error_code ec, std::size_t length){
        if (!ec) {
            do_write(length);
        }
    });
}

void Session::do_write(size_t length) {
    auto self(shared_from_this());
    
    std::istream input(&buffer_);
    std::string command;
    std::getline(input, command);
    std::string response = process_command(command);
    response += '\n';

    boost::asio::async_write(socket_, boost::asio::buffer(response),
    [this, self](boost::system::error_code ec, size_t){
        if (!ec) {
            do_read();
        }
    });
}

std::string Session::process_command(const std::string& input) {
    auto command = Parser::parse(input);
    if (!command)
        return "ERROR: Incorrect command\n";
    if (!command->validate())
        return "ERROR: Validation error\n";
    return command->execute(db_);
}