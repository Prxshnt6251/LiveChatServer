#include "ChatServer.hpp"
#include "WebSocketSession.hpp"
#include <iostream>

using std::cout;
using std::cerr;
using std::make_shared;
using boost::system::error_code;

ChatServer::ChatServer(net::io_context& io_context, unsigned short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
    cout << "[Server] Started on port " << port << '\n';
    accept();
}

void ChatServer::accept() {
    acceptor_.async_accept([this](error_code error, tcp::socket socket) {
        if (!error) {
            make_shared<WebSocketSession>(std::move(socket), room_)->start();
        } else {
            cerr << "[Server] Accept error: " << error.message() << '\n';
        }

        accept();
    });
}
