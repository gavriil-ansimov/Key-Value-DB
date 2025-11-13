#include <boost/asio.hpp>
#include <iostream>

#include "network/Server.h"

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

        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) {
            io_context.stop();
        });

        Server s(io_context, port);

        std::cout << "Key-Value storage server running on port " << port << std::endl;
        std::cout << "Press Ctrl+C to stop\n";

        io_context.run();

        std::cout << "Server stopped gracefully\n";

    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
