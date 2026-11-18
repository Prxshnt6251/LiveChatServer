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

## 🚀 Getting Started on Your Machine (Clone & Run)

Follow these steps if you just cloned this repository onto a fresh computer.

### Step 1: Install Prerequisites

You need a C++17 compiler, CMake, and the Boost libraries.

#### On macOS:
Using [Homebrew](https://brew.sh):
```bash
brew install cmake boost
```
*(Both Apple Silicon M1/M2/M3/M4 and Intel Macs are fully supported).*

#### On Ubuntu / Debian:
```bash
sudo apt update
sudo apt install -y build-essential cmake libboost-all-dev
```

#### On Fedora / RHEL:
```bash
sudo dnf install -y gcc-c++ cmake boost-devel
```

#### On Arch Linux:
```bash
sudo pacman -S base-devel cmake boost
```

---

### Step 2: Clone the Repository

```bash
git clone https://github.com/Prxshnt6251/LiveChatServer.git
cd LiveChatServer
```

---

### Step 3: Run with the Unified Launcher (`run_demo.sh`)

The script automatically detects if the project needs compilation, builds it using CMake, and starts the requested component.

Make sure the script has execution permissions:
```bash
chmod +x run_demo.sh
```

#### Running the Demo (Open 3 Terminal Windows or Tabs):

1. **Terminal Window 1 — Start the Server:**
   ```bash
   ./run_demo.sh
   ```
   *Output:*
   ```text
   ==========================================================
          C++ Live Chat Broadcast Server (Port 8080)
   ==========================================================
   Server starting! In other terminal windows, run:
      ./run_demo.sh Alice
      ./run_demo.sh Bob
   Press Ctrl+C to stop the server.
   ==========================================================
   [Server] Started on port 8080
   [Server] Ready for connections. Type message here to broadcast as admin.
   ```

2. **Terminal Window 2 — Connect as User "Alice":**
   ```bash
   ./run_demo.sh Alice
   ```

3. **Terminal Window 3 — Connect as User "Bob":**
   ```bash
   ./run_demo.sh Bob
   ```

---

### Step 4: Test Broadcasting

- Go to **Terminal 2 (Alice)**, type:
  ```text
  Hello Bob! Testing real-time broadcast.
  ```
  Press **Enter**.
- Check **Terminal 3 (Bob)**: The message appears immediately in real time:
  ```text
  [Alice]: Hello Bob! Testing real-time broadcast.
  ```
- Type a reply in **Terminal 3 (Bob)** $\rightarrow$ it instantly appears in Alice's terminal!
- Type a message in **Terminal 1 (Server)** $\rightarrow$ it broadcasts a `[Server Admin]` announcement to all connected clients!

---

## Manual Build with CMake (Alternative)

If you prefer building manually without the shell script:

```bash
# 1. Configure CMake build directory
cmake -B backend/build -S backend

# 2. Compile both chat_server and chat_client
cmake --build backend/build
```

After building:
- Start server: `./backend/build/chat_server`
- Start client: `./backend/build/chat_client <Username>`

---

## 🛠️ VS Code Configuration (Zero-Configuration IntelliSense)

This repository includes pre-configured `.vscode/` and `.clangd` files:
- **`.vscode/c_cpp_properties.json`**: Configured for macOS and Linux include paths (`/opt/homebrew/include`, `/usr/local/include`) and linked to `compile_commands.json`.
- **`.vscode/settings.json`**: Points CMake Tools extension directly to `${workspaceFolder}/backend`.
- **`.clangd`**: Language server database configuration for error-free indexing.

When opening in VS Code:
1. Open the folder: `code .`
2. If prompted, select your compiler (Clang or GCC).
3. Headers like `<boost/asio.hpp>` and `<boost/beast.hpp>` will resolve without red squiggly lines.

---

## ❓ Troubleshooting & FAQs for New Users

### 1. `boost/asio.hpp: No such file or directory`
- **Cause**: Boost is not installed on the system, or CMake cannot locate it.
- **Fix**: Run `brew install boost` (macOS) or `sudo apt install libboost-all-dev` (Ubuntu). Then re-run `cmake -B backend/build -S backend`.

### 2. `Address already in use (port 8080)`
- **Cause**: Another instance of `chat_server` or another app is already using port `8080`.
- **Fix**: Free the port by running:
  ```bash
  lsof -ti:8080 | xargs kill -9
  ```

### 3. `Permission denied: ./run_demo.sh`
- **Fix**: Grant executable permission:
  ```bash
  chmod +x run_demo.sh
  ```

### 4. How to Disconnect or Stop
- Client: Type `exit` or `quit` and press **Enter**.
- Server: Press **`Ctrl + C`** in the server terminal window.

---

## Documentation & Presentation Guide

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
├── README.md                       # Complete project setup and operation guide
└── run_demo.sh                     # Unified demo launcher script
```

---

## Author

- **Prashant Saini** — [GitHub](https://github.com/Prxshnt6251) &bull; [parshantsaini360@gmail.com](mailto:parshantsaini360@gmail.com)
- **Pratyaksh Semwal** — [GitHub](https://github.com/pratyaksho7) &bull; []()
