#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "8080");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected to server. Type commands:\n";

        while (true) {
            std::cout << "> ";
            std::string cmd;
            std::getline(std::cin, cmd);
            cmd += '\n';
            boost::asio::write(socket, boost::asio::buffer(cmd));
            
            boost::asio::streambuf response;
            boost::asio::read_until(socket, response, '\n');

            if (cmd == "EXIT")
                break;

            std::istream input(&response);
            std::string reply;
            std::getline(input, reply);

            std::cout << "Server: " << reply << std::endl;
        }

        socket.close();
    } catch (std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
    }

    return 0;
}