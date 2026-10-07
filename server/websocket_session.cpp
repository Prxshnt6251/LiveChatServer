#include "websocket_session.hpp"
#include <iostream>
#include <boost/asio/post.hpp>
#include <boost/asio/buffer.hpp>

websocket_session::websocket_session(tcp::socket socket, std::shared_ptr<room_manager> rooms)
    : ws_(std::move(socket)), rooms_(std::move(rooms)) {
}

websocket_session::~websocket_session() {
    rooms_->leave_all(shared_from_this());
}

void websocket_session::run() {
    ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));
    ws_.set_option(websocket::stream_base::decorator(
        [](websocket::response_type& res) {
            res.set(beast::http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " chat-server");
        }));

    ws_.async_accept(
        beast::bind_front_handler(&websocket_session::on_accept, shared_from_this()));
}

void websocket_session::on_accept(beast::error_code ec) {
    if (ec) {
        std::cerr << "accept error: " << ec.message() << "\n";
        return;
    }
    do_read();
}

void websocket_session::do_read() {
    ws_.async_read(
        buffer_,
        beast::bind_front_handler(&websocket_session::on_read, shared_from_this()));
}

void websocket_session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec == websocket::error::closed) {
        return;
    }
    if (ec) {
        std::cerr << "read error: " << ec.message() << "\n";
        return;
    }

    std::string msg = beast::buffers_to_string(buffer_.data());
    buffer_.consume(buffer_.size());

    try {
        auto j = nlohmann::json::parse(msg);
        std::string type = j.value("type", "");

        if (type == "join") {
            std::string room = j.value("room", "");
            if (!room.empty()) {
                rooms_->join(room, shared_from_this());
                std::cout << "User joined room: " << room << "\n";
            }
        } else if (type == "msg") {
            std::string room = j.value("room", "");
            if (!room.empty()) {
                rooms_->broadcast(room, j);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
    }

    do_read();
}

void websocket_session::send(std::shared_ptr<std::string const> const& ss) {
    asio::post(ws_.get_executor(), [self = shared_from_this(), ss]() {
        bool writing = false;
        {
            std::lock_guard<std::mutex> lock(self->queue_mutex_);
            writing = !self->queue_.empty();
            self->queue_.push_back(ss);
        }
        if (!writing) {
            self->ws_.async_write(
                asio::buffer(*self->queue_.front()),
                beast::bind_front_handler(&websocket_session::on_write, self));
        }
    });
}

void websocket_session::on_write(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec) {
        std::cerr << "write error: " << ec.message() << "\n";
        return;
    }

    std::lock_guard<std::mutex> lock(queue_mutex_);
    queue_.erase(queue_.begin());

    if (!queue_.empty()) {
        ws_.async_write(
            asio::buffer(*queue_.front()),
            beast::bind_front_handler(&websocket_session::on_write, shared_from_this()));
    }
}
