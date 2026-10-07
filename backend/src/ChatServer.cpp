#include "ChatServer.hpp"
#include <iostream>

ChatServer::ChatServer(boost::asio::io_context& io_context,
                       unsigned short port)
    : acceptor_(
          io_context,
          boost::asio::ip::tcp::endpoint(
              boost::asio::ip::tcp::v4(),
              port))
{
    std::cout << "Server started on port "
              << port << std::endl;
}