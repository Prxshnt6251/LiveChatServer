#pragma once

#include "room_manager.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/dispatch.hpp>
#include <memory>
#include <string>
#include <vector>
#include <mutex>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

class websocket_session : public std::enable_shared_from_this<websocket_session> {
public:
    explicit websocket_session(tcp::socket socket, std::shared_ptr<room_manager> rooms);
    ~websocket_session();

    void run();
    void send(std::shared_ptr<std::string const> const& ss);

private:
    void on_accept(beast::error_code ec);
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void on_write(beast::error_code ec, std::size_t bytes_transferred);

    websocket::stream<beast::tcp_stream> ws_;
    std::shared_ptr<room_manager> rooms_;
    beast::flat_buffer buffer_;
    std::vector<std::shared_ptr<std::string const>> queue_;
    std::mutex queue_mutex_;
};
