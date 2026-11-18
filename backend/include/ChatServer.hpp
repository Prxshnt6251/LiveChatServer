#pragma once

#include <boost/asio.hpp>
#include "ChatRoom.hpp"

namespace net = boost::asio;
using tcp = net::ip::tcp;

class ChatServer {
public:
    ChatServer(net::io_context& io_context, unsigned short port);
    ChatRoom& get_room() { return room_; }

private:
    tcp::acceptor acceptor_;
    ChatRoom room_;

    void accept();
};
