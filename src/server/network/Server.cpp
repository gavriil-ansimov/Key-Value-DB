#include "Server.h"

#include "../concurrency/WorkerPool.h"
#include "../logging/ILogger.h"

Server::Server(boost::asio::io_context& accept_io, unsigned short port, WorkerPool& workers, ILogger& logger)
    : acceptor_(accept_io, tcp::endpoint(tcp::v4(), port))
    , workers_(workers), db_(logger), logger_(logger)
    {
        logger_.log(LogLevel::Info
        , "Key-Value storage server running on port " + std::to_string(acceptor_.local_endpoint().port()));
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
                        std::make_shared<Session>(std::move(s), db_, logger_)->start();
                    }
                );
            } else if (ec == boost::asio::error::operation_aborted) {
                logger_.log(LogLevel::Info, "Accept operation cancelled");
            } else {
                logger_.log(LogLevel::Info, "Socket error: " + ec.message());
            }
            do_accept();
        }
    );
}