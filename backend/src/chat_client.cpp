#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::string;
using std::thread;
using std::exception;

namespace net = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = net::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        string username = (argc > 1) ? argv[1] : "Anonymous";
        string host = "127.0.0.1";
        string port = "8080";

        net::io_context io_context;
        tcp::resolver resolver(io_context);
        websocket::stream<tcp::socket> ws(io_context);

        // Connect to server
        auto const endpoints = resolver.resolve(host, port);
        net::connect(ws.next_layer(), endpoints);

        // Perform WebSocket handshake
        ws.handshake(host, "/");

        cout << "====================================================\n";
        cout << " Connected to Live Chat Server (" << host << ":" << port << ")\n";
        cout << " User: [" << username << "]\n";
        cout << " Type message & press Enter to broadcast. ('exit' to quit)\n";
        cout << "====================================================\n\n";

        // Background thread to read and print incoming broadcast messages
        thread reader([&ws]() {
            try {
                while (true) {
                    beast::flat_buffer buffer;
                    ws.read(buffer);
                    cout << beast::make_printable(buffer.data()) << '\n';
                }
            } catch (const exception&) {
                cout << "\n[Disconnected from server]\n";
            }
        });

        // Main thread: read user text from terminal stdin
        string line;
        while (getline(cin, line)) {
            if (line == "exit" || line == "quit") {
                break;
            }
            if (!line.empty()) {
                string formatted = "[" + username + "]: " + line;
                ws.write(net::buffer(formatted));
            }
        }

        ws.close(websocket::close_code::normal);
        if (reader.joinable()) {
            reader.join();
        }
    } catch (const exception& e) {
        cerr << "Connection error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
