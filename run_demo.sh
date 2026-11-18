#!/usr/bin/env bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

# Build binaries if missing
if [[ ! -f "$DIR/backend/build/chat_server" ]] || [[ ! -f "$DIR/backend/build/chat_client" ]]; then
    echo "==> Building C++ chat server and client..."
    cmake -B backend/build -S backend > /dev/null
    cmake --build backend/build > /dev/null
fi

if [[ $# -eq 0 ]]; then
    echo "=========================================================="
    echo "       C++ Live Chat Broadcast Server (Port 8080)"
    echo "=========================================================="
    echo "Server starting! In other terminal windows, run:"
    echo "   ./run_demo.sh Alice"
    echo "   ./run_demo.sh Bob"
    echo "Press Ctrl+C to stop the server."
    echo "=========================================================="
    exec "$DIR/backend/build/chat_server"
else
    USER_NAME="$1"
    exec "$DIR/backend/build/chat_client" "$USER_NAME"
fi
