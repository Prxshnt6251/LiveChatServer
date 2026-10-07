#include <iostream>
#include <boost/asio.hpp>
#include "ChatServer.hpp"

int main() {
    try {
        boost::asio::io_context io_context;

        ChatServer server(io_context, 8080);

        io_context.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}