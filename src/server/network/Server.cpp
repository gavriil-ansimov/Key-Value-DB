#include "Server.h"


Server::Server(boost::asio::io_context& accept_io, unsigned short port, WorkerPool& workers)
    : acceptor_(accept_io, tcp::endpoint(tcp::v4(), port))
    , workers_(workers)
    {
        do_accept();
    }

void Server::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket){
            if (!ec) {
                auto& io = workers_.next_io();
                boost::asio::post(
                    io,
                    [s = std::move(socket), this]() mutable {
                        std::make_shared<Session>(std::move(s), db_)->start();
                    }
                );
            }
            do_accept();
        }
    );
}