#include <boost/asio.hpp>
#include <iostream>

#include "network/Server.h"
#include "concurrency/WorkerPool.h"
#include "logging/Logger.h"

int main(int argc, char* argv[]) {
    unsigned short port = 8080;
    int opt;

    Logger logger(LogLevel::Debug);

    while( (opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
        case 'p':
            port = std::atoi(optarg);
            break;
        }
    }
    try {

        logger.log(LogLevel::Info, "Starting server...");

        boost::asio::io_context io_context;

        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) {
            io_context.stop();
        });

        auto threads = std::thread::hardware_concurrency();
        WorkerPool workers(threads);

        logger.log(LogLevel::Info, "Starting worker pool with " + std::to_string(threads) + " threads");

        Server s(io_context, port, workers, logger);

        io_context.run();

        logger.log(LogLevel::Info, "Server stopped gracefully");
    }
    catch (std::exception& e) {
        logger.log(LogLevel::Fatal, std::string("Unhandled exception in main: ") + e.what());
    }
    return 0;
}
