#ifndef WEBSOCKETSESSION_HPP
#define WEBSOCKETSESSION_HPP

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include <memory>

class WebSocketSession
    : public std::enable_shared_from_this<WebSocketSession>
{
public:
    explicit WebSocketSession(
        boost::asio::ip::tcp::socket socket
    );

    void start();

private:
    boost::beast::websocket::stream<
        boost::asio::ip::tcp::socket
    > ws_;

    boost::beast::flat_buffer buffer_;

    void read();
};

#endif