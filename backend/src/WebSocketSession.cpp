#include "WebSocketSession.hpp"
#include "ChatRoom.hpp"
#include <iostream>

using std::cout;
using std::cerr;
using std::make_shared;
using std::shared_ptr;
using std::size_t;
using std::string;

namespace websocket = beast::websocket;
using error_code = boost::system::error_code;

WebSocketSession::WebSocketSession(tcp::socket socket, ChatRoom& room)
    : ws_(std::move(socket))
    , room_(room)
{
}

void WebSocketSession::start() {
    auto self = shared_from_this();

    ws_.async_accept([self](error_code error) {
        if (error) {
            cerr << "[Server] WebSocket handshake failed: " << error.message() << '\n';
            return;
        }

        self->room_.join(self);
        self->read();
    });
}

void WebSocketSession::read() {
    auto self = shared_from_this();

    ws_.async_read(buffer_, [self](error_code error, size_t) {
        if (error == websocket::error::closed || error) {
            self->room_.leave(self);
            return;
        }

        string message = beast::buffers_to_string(self->buffer_.data());
        cout << "[Broadcast] " << message << '\n';

        self->buffer_.consume(self->buffer_.size());
        self->room_.broadcast(message);
        self->read();
    });
}

void WebSocketSession::send(shared_ptr<const string> message) {
    auto self = shared_from_this();
    net::post(ws_.get_executor(), [self, message]() {
        self->queue_.push_back(message);
        if (self->queue_.size() > 1) {
            return;
        }
        self->do_write();
    });
}

void WebSocketSession::do_write() {
    ws_.text(true);
    auto self = shared_from_this();

    ws_.async_write(
        net::buffer(*queue_.front()),
        [self](error_code write_error, size_t) {
            if (write_error) {
                self->room_.leave(self);
                return;
            }

            self->queue_.erase(self->queue_.begin());
            if (!self->queue_.empty()) {
                self->do_write();
            }
        }
    );
}
