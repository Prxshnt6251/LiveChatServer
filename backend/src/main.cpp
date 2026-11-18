#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "ChatServer.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::string;
using std::thread;
using std::exception;

int main() {
    try {
        net::io_context io_context;
        ChatServer server(io_context, 8080);

        cout << "[Server] Ready for connections. Type message here to broadcast as admin.\n";

        // Allows typing directly in the server terminal to broadcast
        thread server_input([&server]() {
            string line;
            while (getline(cin, line)) {
                if (!line.empty()) {
                    server.get_room().broadcast("[Server Admin]: " + line);
                }
            }
        });
        server_input.detach();

        io_context.run();
    } catch (const exception& e) {
        cerr << "[Server] Error: " << e.what() << '\n';
    }

    return 0;
}
