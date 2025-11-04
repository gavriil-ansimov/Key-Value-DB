#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include "Database.h"
#include "Parser.h"

using boost::asio::ip::tcp;

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

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
        Database db;
        Parser parser;
        std::cout << "Server listening...\n";

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout << "Client connected.\n";

            try {
                for(;;) {
                    boost::asio::streambuf buffer;
                    boost::asio::read_until(socket, buffer, '\n');

                    std::istream input(&buffer);
                    std::string command;
                    std::getline(input, command);

                    if (command.empty())
                        continue;
                    
                    std::cout << "Received command:" << command << std::endl;

                    if (command == "EXIT") {
                        std::cout << "Client closed connection" << std::endl;
                        break;
                    }

                    std::string response = parser.parse(command, db);
                    response += '\n';

                    boost::asio::write(socket, boost::asio::buffer(response));
                }
            } catch (std::exception& e) {
                std::cout << "Client disconnected: " << e.what() << std::endl;
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
    return 0;
}