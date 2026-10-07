cmake_minimum_required(VERSION 3.17)
project(LiveChatServer)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

find_package(nlohmann_json REQUIRED)

add_executable(chat_server
    main.cpp
    room_manager.cpp
    websocket_session.cpp
)

target_link_libraries(chat_server PRIVATE
    Boost::boost
    Boost::system
    Threads::Threads
    nlohmann_json::nlohmann_json
)
