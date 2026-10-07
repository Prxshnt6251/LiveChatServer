#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <nlohmann/json.hpp>

class websocket_session;

class room_manager {
public:
    void join(const std::string& room, std::shared_ptr<websocket_session> session);
    void leave(const std::string& room, std::shared_ptr<websocket_session> session);
    void leave_all(std::shared_ptr<websocket_session> session);
    void broadcast(const std::string& room, const nlohmann::json& message);

private:
    std::mutex mutex_;
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<websocket_session>>> rooms_;
    std::unordered_map<std::shared_ptr<websocket_session>, std::unordered_set<std::string>> session_rooms_;
};
