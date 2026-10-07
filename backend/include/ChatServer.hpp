#ifndef CHATSERVER_HPP
#define CHATSERVER_HPP

#include <boost/asio.hpp>

class ChatServer {
public:
    ChatServer(boost::asio::io_context& io_context,
               unsigned short port);

private:
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif