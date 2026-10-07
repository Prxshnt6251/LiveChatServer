cmake_minimum_required(VERSION 3.16)

project(LiveChatServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

add_executable(chat_server
    src/main.cpp
    src/ChatServer.cpp
    src/WebSocketSession.cpp
)

target_include_directories(chat_server PRIVATE
    include
)

target_link_libraries(chat_server PRIVATE
    Boost::system
    Threads::Threads
)