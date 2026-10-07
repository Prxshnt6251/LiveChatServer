# Progress Evaluation Report

This document outlines the current state of the Live Chat Server project, detailing the stack used, the core concepts to understand for the evaluation, and the roadmap for the remaining features.

## 1. Technologies & Architecture Used So Far

### Backend (C++17)
*   **Boost.Asio**: Provides the core asynchronous event loop (`io_context`). It handles the low-level socket connections and concurrency.
*   **Boost.Beast**: Built on top of Asio, this library handles the WebSocket protocol implementation (HTTP upgrade, framing, masking).
*   **nlohmann/json**: A modern C++ JSON library used for seamlessly parsing incoming client messages and stringifying outbound broadcasts.
*   **CMake**: The industry-standard build system used to manage dependencies (like finding Boost and nlohmann_json) and compile the project.

### Frontend
*   **React + Vite**: A lightning-fast modern frontend stack. React handles the UI state (messages, connection status, inputs), while Vite provides the build tooling and dev server.
*   **Vanilla CSS**: Used to create a rich, dark-themed, glassmorphic UI with micro-animations, completely from scratch without utility frameworks.
*   **Native WebSocket API**: The browser's built-in `WebSocket` class is used to maintain the connection with the C++ server.

---

## 2. Key Concepts to Learn & Review (For Your Evaluation)

If you are asked questions during the progress evaluation, focus on these critical design decisions:

*   **Asynchronous I/O & The "One-Write-At-A-Time" Rule**: 
    Boost.Beast WebSockets do not allow concurrent write operations. If two threads try to write to the same socket simultaneously, it will crash. *Solution:* We implemented an outbound `queue_` and use `boost::asio::post` combined with a mutex in `websocket_session.cpp` to ensure writes are queued and dispatched safely.
*   **Memory Management (Smart Pointers)**: 
    The server relies heavily on `std::shared_ptr`. Classes inherit from `std::enable_shared_from_this` to pass a safely reference-counted pointer to Asio's async callbacks. This guarantees that a connection object isn't destroyed while a network read/write is still pending.
*   **Thread Safety in Room Manager**: 
    Because multiple socket connections might try to join/leave rooms or broadcast messages at the exact same time, the `room_manager` uses `std::mutex` and `std::lock_guard` to protect its internal maps (`rooms_` and `session_rooms_`).
*   **React Component Lifecycle**: 
    In the frontend, the WebSocket connection is managed inside a `useEffect` hook. A cleanup function `return () => { ws.current.close() }` ensures that if the component unmounts, we don't leave zombie connections hanging around.

---

## 3. What Will Be Implemented After This Evaluation

Once this progress evaluation is passed, the second half of the project blueprint will be implemented:

1.  **SQLite Message History (Phase 5)**:
    *   Integrate SQLite3 (C API) into the C++ server.
    *   Create a `storage` class that writes every incoming `msg` event to a local `.db` file using prepared SQL statements.
    *   Update the `join` logic so that when a user joins a room, the server immediately queries SQLite and sends the last 50 messages to that specific user.
2.  **Presence Indicators & Heartbeats (Phase 6)**:
    *   Update the `room_manager` to keep track of usernames, not just raw socket sessions.
    *   Broadcast a `{"type":"presence", "users": [...]}` JSON array every time someone joins or leaves a room.
    *   Implement WebSocket ping/pong frames to detect and prune "dead" connections (e.g., users who closed their laptop without cleanly disconnecting).
3.  **Client-Side Polish (Phase 7)**:
    *   Add a "User is typing..." indicator.
    *   Implement automatic reconnect logic in React if the WebSocket drops.
    *   Add input validation (preventing empty messages, sanitizing HTML if necessary).
4.  **Testing (Phase 8)**:
    *   Write a Python or JS load-testing script to simulate 100-500 concurrent connections to ensure the C++ server doesn't buckle under pressure.
