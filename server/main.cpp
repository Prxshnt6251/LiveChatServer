#include "websocket_session.hpp"
#include "room_manager.hpp"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <iostream>
#include <memory>
#include <thread>

void do_accept(tcp::acceptor& acceptor, tcp::socket& socket, std::shared_ptr<room_manager> rooms) {
    acceptor.async_accept(socket, [&acceptor, &socket, rooms](beast::error_code ec) {
        if (!ec) {
            std::make_shared<websocket_session>(std::move(socket), rooms)->run();
        } else {
            std::cerr << "accept error: " << ec.message() << "\n";
        }
        do_accept(acceptor, socket, rooms);
    });
}

int main(int argc, char* argv[]) {
    try {
        auto const address = boost::asio::ip::make_address("0.0.0.0");
        auto const port = static_cast<unsigned short>(8080);

        boost::asio::io_context ioc{1};

        auto rooms = std::make_shared<room_manager>();
        tcp::acceptor acceptor{ioc, {address, port}};
        tcp::socket socket{ioc};

        std::cout << "Starting server on ws://" << address << ":" << port << "...\n";
        do_accept(acceptor, socket, rooms);

        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&](beast::error_code const&, int) {
            ioc.stop();
        });

        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
