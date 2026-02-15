#include "Server.h"


Server::Server(boost::asio::io_context& accept_io, unsigned short port, WorkerPool& workers)
    : acceptor_(accept_io, tcp::endpoint(tcp::v4(), port))
    , workers_(workers)
    {
        LOG_INFO << "Key-Value storage server running on port " << acceptor_.local_endpoint().port();
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
            } else if (ec == boost::asio::error::operation_aborted) {
                LOG_INFO << "Accept operation cancelled";
            } else {
                LOG_ERROR << "Socket error: " << ec.message();
            }
            do_accept();
        }
    );
}