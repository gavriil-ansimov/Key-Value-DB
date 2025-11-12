#include <boost/asio.hpp>
#include <boost/asio/read_until.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <unordered_map>

#include "Database.h"

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session> {
public:
    session(tcp::socket socket, Database& db)
    : socket_(std::move(socket)), db_(db) {}

    void start() {
        do_read();
    }
private:
    void do_read() {
        auto self(shared_from_this());

        boost::asio::async_read_until(socket_, buffer_, '\n',
        [this, self](boost::system::error_code ec, std::size_t length){
            if (!ec) {
                do_write(length);
            }
        });
    }

    void do_write(std::size_t length) {
        auto self(shared_from_this());
        
        std::istream input(&buffer_);
        std::string command;
        std::getline(input, command);
        std::string response = db_.execute_command(command);
        response += '\n';

        boost::asio::async_write(socket_, boost::asio::buffer(response),
        [this, self](boost::system::error_code ec, size_t){
            if (!ec) {
                do_read();
            }
        });
    }

    tcp::socket socket_;
    boost::asio::streambuf buffer_;
    Database& db_;
};

class server {
public:
    server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      socket_(io_context) {
        do_accept();
      }
private:
    void do_accept() {
        acceptor_.async_accept(socket_,
        [this](boost::system::error_code ec){
            if (!ec) {
                std::make_shared<session>(std::move(socket_), db_)->start();
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    Database db_;
};

int main(int argc, char* argv[]) {
    unsigned short port = 8080;
    int opt;

    while( (opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
        case 'p':
            port = std::atoi(optarg);
            break;
        default:
            std::cout << "Using default port: " << port << std::endl;
        }
    }
    try {
        boost::asio::io_context io_context;
        server s(io_context, port);
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
