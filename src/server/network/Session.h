#pragma once

#include <boost/asio.hpp>
#include <memory>

class Database;
class CommandParser;
class ILogger;

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, Database& db, ILogger& logger);
    ~Session();
    void start();
private:
    void do_read();
    void do_write(size_t);
    std::string process_command(const std::string& input);

    tcp::socket socket_;
    boost::asio::streambuf buffer_;
    Database& db_;
    ILogger& logger_;
    std::string clientAddr_;
};