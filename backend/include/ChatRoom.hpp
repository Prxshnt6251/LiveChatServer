#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_set>

class WebSocketSession;

class ChatRoom {
public:
    void join(std::shared_ptr<WebSocketSession> session);
    void leave(std::shared_ptr<WebSocketSession> session);
    void broadcast(const std::string& message);

private:
    std::mutex mutex_;
    std::unordered_set<std::shared_ptr<WebSocketSession>> sessions_;
};
