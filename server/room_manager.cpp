#include "room_manager.hpp"
#include "websocket_session.hpp"

void room_manager::join(const std::string& room, std::shared_ptr<websocket_session> session) {
    std::lock_guard<std::mutex> lock(mutex_);
    rooms_[room].insert(session);
    session_rooms_[session].insert(room);
}

void room_manager::leave(const std::string& room, std::shared_ptr<websocket_session> session) {
    std::lock_guard<std::mutex> lock(mutex_);
    rooms_[room].erase(session);
    session_rooms_[session].erase(room);
}

void room_manager::leave_all(std::shared_ptr<websocket_session> session) {
    std::vector<std::string> rooms_left;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = session_rooms_.find(session);
        if (it != session_rooms_.end()) {
            for (const auto& room : it->second) {
                rooms_[room].erase(session);
                rooms_left.push_back(room);
            }
            session_rooms_.erase(it);
        }
    }
    
    std::string user = session->get_username();
    if (!user.empty()) {
        nlohmann::json sysMsg = {
            {"type", "system"},
            {"text", user + " has left the room."}
        };
        for (const auto& room : rooms_left) {
            broadcast(room, sysMsg);
        }
    }
}

void room_manager::broadcast(const std::string& room, const nlohmann::json& message) {
    auto const ss = std::make_shared<std::string const>(message.dump());

    std::vector<std::shared_ptr<websocket_session>> targets;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = rooms_.find(room);
        if (it != rooms_.end()) {
            for (auto& session : it->second) {
                targets.push_back(session);
            }
        }
    }

    for (auto& session : targets) {
        session->send(ss);
    }
}
