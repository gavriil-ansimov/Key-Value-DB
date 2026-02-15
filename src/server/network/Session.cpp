#include "Session.h"
#include "../database/Database.h"
#include "../command/Parser.h"

#include <iostream>
#include <sstream>

Session::Session(tcp::socket socket, Database& db)
    : socket_(std::move(socket)), db_(db) {
        LOG_INFO << "Cleint connected: " << socket_.remote_endpoint();
    }

Session::~Session() {
    boost::system::error_code ec;
    auto ep = socket_.remote_endpoint(ec);
    if (!ec)
        LOG_INFO << "Client disconnected: " << ep;
    else
        LOG_DEBUG << "Client disconnected";
}

void Session::start() {
    do_read();
}

void Session::do_read() {
    auto self(shared_from_this());

    boost::asio::async_read_until(socket_, buffer_, '\n',
    [this, self](boost::system::error_code ec, std::size_t length){
        if (!ec) {
            LOG_DEBUG << "Received " << length << " bytes from " << socket_.remote_endpoint();
            do_write(length);
        } else {
            LOG_ERROR << "Read error: " << ec.message();
        }
    });
}

void Session::do_write(size_t length) {
    auto self(shared_from_this());
    
    std::istream input(&buffer_);
    std::string command;
    std::getline(input, command);

    auto response = std::make_shared<std::string>(process_command(command) + '\n');

    boost::asio::async_write(socket_, boost::asio::buffer(*response),
    [this, self](boost::system::error_code ec, size_t length){
        if (!ec) {
            LOG_DEBUG << "Send " << length << " bytes to " << socket_.remote_endpoint();
            do_read();
        } else {
            LOG_ERROR << "Write error: " << ec.message();
        }
    });
}

std::string Session::process_command(const std::string& input) {
    auto command = Parser::parse(input);
    if (!command) {
        LOG_WARNING << "Incorrect command received";
        return "ERROR: Incorrect command\n";
    }
    if (!command->validate()) {
        LOG_WARNING << "Command validation failed";
        return "ERROR: Validation error\n";
    }
    return command->execute(db_);
}