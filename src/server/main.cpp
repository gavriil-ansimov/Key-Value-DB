#include <boost/asio.hpp>
#include <iostream>

#include "network/Server.h"
#include "concurrency/WorkerPool.h"
#include "logging/Logger.h"

int main(int argc, char* argv[]) {
    unsigned short port = 8080;
    int opt;

    while( (opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
        case 'p':
            port = std::atoi(optarg);
            break;
        default:
            LOG_INFO << "Using default port: " << port;
        }
    }
    try {
        logging::init();

        LOG_INFO << "Starting server...";

        boost::asio::io_context io_context;

        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) {
            io_context.stop();
        });

        auto threads = std::thread::hardware_concurrency();
        WorkerPool workers(threads);

        LOG_INFO << "Starting worker pool with " << threads << " threads";

        Server s(io_context, port, workers);

        io_context.run();

        LOG_INFO << "Server stopped gracefully";

    }
    catch (std::exception& e) {
        LOG_FATAL << "Unhandled exception in main: " << e.what() << std::endl;
    }
    return 0;
}
