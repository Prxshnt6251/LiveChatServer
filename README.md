# LiveChatServer

A high-performance, asynchronous multi-user live chat collaboration server and interactive terminal client built with **modern C++17** and **WebSockets (RFC 6455)** powered by **Boost.Beast** and **Boost.Asio**.

---

## Architecture Overview

The system operates on an asynchronous non-blocking event-driven model (`net::io_context`) capable of handling multiple concurrent clients without the overhead of a thread-per-client architecture:

```
+--------------------------------------------------------------------------------+
|                         TERMINAL BROADCAST DATAFLOW                            |
+--------------------------------------------------------------------------------+
  [ Terminal Window 1 (Alice) ] --(Type & Enter)--> [ C++ chat_server (8080) ]   |
                                                             |                   |
                                                      (Broadcast Hub)            |
                                                             v                   |
  [ Terminal Window 2 (Bob) ]   <---(Instant Display)--------+                   |
  [ Terminal Window 3 (Charlie) ] <---(Instant Display)------+                   |
+--------------------------------------------------------------------------------+
```

### Core Components
- **`ChatServer`** (`backend/include/ChatServer.hpp`): Listens on TCP port `8080` using `boost::asio::ip::tcp::acceptor` and accepts incoming client connections asynchronously.
- **`ChatRoom`** (`backend/include/ChatRoom.hpp`): Central broadcast hub. Maintains an active set of connected client sessions protected with `std::mutex` and `std::lock_guard` for safe concurrent operations.
- **`WebSocketSession`** (`backend/include/WebSocketSession.hpp`): Manages the WebSocket RFC 6455 handshake, asynchronous reads, and serializes outbound broadcasts via an internal message queue to prevent write collisions.
- **`chat_client`** (`backend/src/chat_client.cpp`): Multi-threaded C++ terminal client. The main thread reads keyboard input from `std::cin` while a background `std::thread` continuously reads incoming WebSocket frames from the server.
- **`run_demo.sh`**: Unified launcher script providing single-command build and multi-client startup.

---

## Prerequisites

- **C++ Compiler**: Clang (AppleClang >= 12.0) or GCC (>= 9.0) with C++17 support
- **CMake**: Version 3.16 or higher
- **Boost Libraries**: System component (`boost::system`)
  - **macOS**: `brew install boost`
  - **Ubuntu / Debian**: `sudo apt install libboost-all-dev`

---

## Quick Start (Unified Launcher)

The included `run_demo.sh` script automatically checks and compiles the backend, and launches either the server or terminal clients with a single command:

```bash
# Clone the repository
git clone https://github.com/Prxshnt6251/LiveChatServer.git
cd LiveChatServer

# 1. Start the Chat Server (Terminal Window 1)
./run_demo.sh

# 2. Connect Client 1 as Alice (Terminal Window 2)
./run_demo.sh Alice

# 3. Connect Client 2 as Bob (Terminal Window 3)
./run_demo.sh Bob
```

---

## Manual Build Instructions

If you prefer to build manually with CMake:

```bash
# 1. Configure the project
cmake -B backend/build -S backend

# 2. Build executables (chat_server & chat_client)
cmake --build backend/build
```

This generates two executables in `backend/build/`:
- `backend/build/chat_server`
- `backend/build/chat_client`

---

## Live Demo Walkthrough

### 1. Start Server
In your first terminal window:
```bash
./backend/build/chat_server
```
*Console output:*
```text
[Server] Started on port 8080
[Server] Ready for connections. Type message here to broadcast as admin.
```

### 2. Connect Clients
In separate terminal windows:
```bash
# Terminal 2:
./backend/build/chat_client Alice

# Terminal 3:
./backend/build/chat_client Bob
```

### 3. Real-Time Broadcasting
- In **Alice's terminal**, type:
  ```text
  Hello Bob! Testing live C++ broadcast.
  ```
  Press **Enter**.
- In **Bob's terminal**, the message immediately appears:
  ```text
  [Alice]: Hello Bob! Testing live C++ broadcast.
  ```
- Any reply typed in **Bob's terminal** instantly broadcasts back to Alice.
- Typing directly in the **Server console** broadcasts a `[Server Admin]` announcement to all connected clients!

---

## Documentation & Teacher Presentation Guide

A comprehensive 3-page presentation walkthrough and technical defense guide is included:
- **[Live_Chat_Collaboration_Demo_Guide.pdf](Live_Chat_Collaboration_Demo_Guide.pdf)**

It includes:
1. Complete step-by-step presentation script for academic evaluation.
2. Architecture and dataflow diagrams.
3. Model answers to anticipated technical defense questions (non-blocking event loops, mutex synchronization, WebSockets vs raw sockets).

---

## Project Structure

```text
LiveChatServer/
├── backend/
│   ├── CMakeLists.txt              # Build configuration for server & client
│   ├── include/
│   │   ├── ChatRoom.hpp            # Thread-safe broadcast room manager
│   │   ├── ChatServer.hpp          # Asynchronous TCP acceptor
│   │   └── WebSocketSession.hpp    # WebSocket RFC 6455 session handler
│   └── src/
│       ├── ChatRoom.cpp            # Room membership & broadcast logic
│       ├── ChatServer.cpp          # TCP server implementation
│       ├── WebSocketSession.cpp    # Async read/write queue handling
│       ├── chat_client.cpp         # Interactive terminal client
│       └── main.cpp                # Server entry point + admin broadcast
├── .clangd                         # Language server compilation flags
├── .gitignore                      # Git ignore patterns for build & OS artifacts
├── Live_Chat_Collaboration_Demo_Guide.pdf  # Teacher presentation guide
├── README.md                       # Project documentation
└── run_demo.sh                     # Unified demo launcher script
```

---

## Author

- **Prashant Saini** — [GitHub](https://github.com/Prxshnt6251) &bull; [parshantsaini360@gmail.com](mailto:parshantsaini360@gmail.com)
