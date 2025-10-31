#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

std::string process_command(const std::string& cmd);

int main(int argc, char* argv[]) {
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));
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
                        std::cout << "Client closed connection";
                        break;
                    }

                    std::string response = process_command(command);
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