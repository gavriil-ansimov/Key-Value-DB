#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "../database/Database.h"
#include "../concurrency/WorkerPool.h"
#include "Session.h"

using boost::asio::ip::tcp;

class Server {
public:
    Server(boost::asio::io_context& accept_io, unsigned short port, WorkerPool& workers);
private:
    void do_accept();

    WorkerPool& workers_;
    tcp::acceptor acceptor_;
    Database db_;
};