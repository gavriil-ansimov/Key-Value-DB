#pragma once

#include <boost/asio.hpp>
#include <memory>

class Database;
class CommandParser;

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, Database& db);
    void start();
private:
    void do_read();
    void do_write(size_t);
    std::string process_command(const std::string& input);

    tcp::socket socket_;
    boost::asio::streambuf buffer_;
    Database& db_;
};