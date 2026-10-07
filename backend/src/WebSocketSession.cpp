#include "WebSocketSession.hpp"

#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace websocket = boost::beast::websocket;
using tcp = boost::asio::ip::tcp;

WebSocketSession::WebSocketSession(tcp::socket socket)
    : ws_(std::move(socket))
{
}

void WebSocketSession::start()
{
    auto self = shared_from_this();

    ws_.async_accept(
        [self](boost::system::error_code error)
        {
            if (error)
            {
                std::cerr
                    << "WebSocket handshake failed: "
                    << error.message()
                    << std::endl;

                return;
            }

            std::cout
                << "WebSocket client connected!"
                << std::endl;

            self->read();
        }
    );
}

void WebSocketSession::read()
{
    auto self = shared_from_this();

    ws_.async_read(
        buffer_,
        [self](boost::system::error_code error,
               std::size_t)
        {
            if (error == websocket::error::closed)
            {
                std::cout
                    << "Client disconnected."
                    << std::endl;

                return;
            }

            if (error)
            {
                std::cerr
                    << "Read error: "
                    << error.message()
                    << std::endl;

                return;
            }

            std::string message =
                beast::buffers_to_string(
                    self->buffer_.data()
                );

            std::cout
                << "Received: "
                << message
                << std::endl;

            self->ws_.text(true);

            auto response =
                std::make_shared<std::string>(message);

            self->ws_.async_write(
                boost::asio::buffer(*response),
                [self, response](
                    boost::system::error_code write_error,
                    std::size_t)
                {
                    if (write_error)
                    {
                        std::cerr
                            << "Write error: "
                            << write_error.message()
                            << std::endl;

                        return;
                    }

                    self->buffer_.consume(
                        self->buffer_.size()
                    );

                    self->read();
                }
            );
        }
    );
}