#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <memory>
#include <string>
#include <vector>

namespace net = boost::asio;
namespace beast = boost::beast;
using tcp = net::ip::tcp;

class ChatRoom;

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
    WebSocketSession(tcp::socket socket, ChatRoom& room);
    void start();
    void send(std::shared_ptr<const std::string> message);

private:
    beast::websocket::stream<tcp::socket> ws_;
    beast::flat_buffer buffer_;
    ChatRoom& room_;
    std::vector<std::shared_ptr<const std::string>> queue_;

    void read();
    void do_write();
};
