cmake_minimum_required(VERSION 3.16)

# Silence FindBoost CMP0167 policy warning in modern CMake
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

project(LiveChatServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

# 1. Chat Server Executable
add_executable(chat_server
    src/main.cpp
    src/ChatServer.cpp
    src/ChatRoom.cpp
    src/WebSocketSession.cpp
)

target_include_directories(chat_server PRIVATE
    include
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(chat_server PRIVATE
    Boost::system
    Threads::Threads
)

# 2. Terminal Chat Client Executable
add_executable(chat_client
    src/chat_client.cpp
)

target_include_directories(chat_client PRIVATE
    include
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(chat_client PRIVATE
    Boost::system
    Threads::Threads
)
