#include "ChatRoom.hpp"
#include "WebSocketSession.hpp"
#include <iostream>

using std::cout;
using std::lock_guard;
using std::make_shared;
using std::mutex;
using std::shared_ptr;
using std::string;

void ChatRoom::join(shared_ptr<WebSocketSession> session) {
    lock_guard<mutex> lock(mutex_);
    sessions_.insert(session);
    cout << "[Server] User joined room. (Active clients: " << sessions_.size() << ")\n";
}

void ChatRoom::leave(shared_ptr<WebSocketSession> session) {
    lock_guard<mutex> lock(mutex_);
    sessions_.erase(session);
    cout << "[Server] User left room. (Active clients: " << sessions_.size() << ")\n";
}

void ChatRoom::broadcast(const string& message) {
    lock_guard<mutex> lock(mutex_);
    auto const msg = make_shared<const string>(message);
    for (const auto& session : sessions_) {
        session->send(msg);
    }
}
