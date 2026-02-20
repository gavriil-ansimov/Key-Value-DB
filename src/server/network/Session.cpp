#include "Session.h"
#include "../database/Database.h"
#include "../command/Parser.h"
#include "../logging/ILogger.h"

#include <iostream>
#include <sstream>

Session::Session(tcp::socket socket, Database& db, ILogger& logger)
    : socket_(std::move(socket)), db_(db), logger_(logger) {
        auto ep = socket_.remote_endpoint();
        clientAddr_ = ep.address().to_string() + ":" + std::to_string(ep.port());
        logger_.log(LogLevel::Info, "Client connected: " + clientAddr_);
    }

Session::~Session() {
    logger_.log(LogLevel::Info, "Client disconnected: " + clientAddr_);
}

void Session::start() {
    do_read();
}

void Session::do_read() {
    auto self(shared_from_this());

    boost::asio::async_read_until(socket_, buffer_, '\n',
    [this, self](boost::system::error_code ec, std::size_t length){
        if (!ec) {
            logger_.log(LogLevel::Debug, "Received " + std::to_string(length) + " bytes from " + clientAddr_);
            do_write(length);
        } else {
            logger_.log(LogLevel::Error, "Read error: " + ec.message());
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
            logger_.log(LogLevel::Debug, "Send " + std::to_string(length) + " bytes to " + clientAddr_);
            do_read();
        } else {
            logger_.log(LogLevel::Error, "Write error: " + ec.message());
        }
    });
}

std::string Session::process_command(const std::string& input) {
    try {
        auto command = Parser::parse(input);
        if (!command) {
            logger_.log(LogLevel::Warning, "Incorrect command received");
            return "ERROR: Incorrect command\n";
        }
        if (!command->validate()) {
            logger_.log(LogLevel::Warning, "Command validation failed");
            return "ERROR: Validation error\n";
        }
        return command->execute(db_);
    }
    catch (std::exception& e) {
        logger_.log(LogLevel::Error, std::string("Command execution failed: ") + e.what());
        return std::string("ERROR: ") + e.what() + '\n';
    }
}