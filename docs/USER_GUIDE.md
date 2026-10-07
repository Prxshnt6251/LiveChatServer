# Live Chat - User & Developer Guide

This guide explains how to compile, run, and test the Live Chat application locally on your machine.

## Prerequisites

Before starting, ensure you have the following installed on your system:
- **C++ Compiler** (supporting C++17)
- **CMake** (version 3.17+)
- **Boost Libraries** (specifically `system` and `thread`)
- **nlohmann/json** (C++ JSON library)
- **Node.js & npm** (for the React frontend)

*(If you are on macOS, you can install the C++ dependencies via Homebrew: `brew install cmake boost nlohmann-json`)*

---

## 1. Running the C++ WebSocket Server

The backend is built with Boost.Beast and Boost.Asio. It listens for WebSocket connections on `ws://0.0.0.0:8080`.

1. Open a terminal and navigate to the `server/` directory:
   ```bash
   cd server
   ```
2. Create a build directory and configure the project:
   ```bash
   mkdir -p build && cd build
   cmake ..
   ```
3. Compile the server:
   ```bash
   make -j
   ```
4. Start the server:
   ```bash
   ./chat_server
   ```
   *You should see an output saying: `Starting server on port 8080...`*

---

## 2. Running the React Frontend

The frontend is a modern React application built with Vite and vanilla CSS.

1. Open a **new** terminal window and navigate to the `client/` directory:
   ```bash
   cd client
   ```
2. Install the necessary NPM dependencies:
   ```bash
   npm install
   ```
3. Start the Vite development server:
   ```bash
   npm run dev
   ```
   *You should see an output with a local URL, typically: `http://localhost:5173`*

---

## 3. How to Use and Test the App

To properly test the real-time broadcasting and room isolation features, follow these steps:

1. **Open the App**: Open [http://localhost:5173](http://localhost:5173) in your web browser.
2. **Join a Room**: 
   * Enter a Username (e.g., `Alice`).
   * Enter a Room Name (e.g., `general`).
   * Click **Join Chat**.
3. **Simulate Multiple Users**:
   * Open a **second tab** or a different browser window to `http://localhost:5173`.
   * Join as a different user (e.g., `Bob`) in the **same room** (`general`).
   * Send a message from Alice. You will instantly see it appear on Bob's screen!
4. **Test Room Isolation**:
   * Open a **third tab**.
   * Join as a new user (e.g., `Charlie`) but put them in a **different room** (e.g., `secret`).
   * Have Charlie send a message. Notice that Alice and Bob (who are in `general`) do *not* receive Charlie's message, proving that the server successfully routes messages only to the intended rooms.

---

## Troubleshooting

- **Server fails to bind**: If the C++ server says "Address already in use", another program is using port `8080`. You will need to stop that program first.
- **Frontend can't connect**: Make sure the C++ server is running. If the frontend says "Disconnected", check the C++ server terminal for any error logs.
