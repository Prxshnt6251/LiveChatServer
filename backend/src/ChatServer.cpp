#include "ChatServer.hpp"
#include "WebSocketSession.hpp"

#include <iostream>

using tcp = boost::asio::ip::tcp;

ChatServer::ChatServer(
    boost::asio::io_context& io_context,
    unsigned short port
)
    : acceptor_(
          io_context,
          tcp::endpoint(
              tcp::v4(),
              port
          )
      )
{
    std::cout
        << "Server started on port "
        << port
        << std::endl;

    accept();
}

void ChatServer::accept()
{
    acceptor_.async_accept(
        [this](
            boost::system::error_code error,
            tcp::socket socket
        )
        {
            if (!error)
            {
                std::cout
                    << "TCP client connected!"
                    << std::endl;

                std::make_shared<WebSocketSession>(
                    std::move(socket)
                )->start();
            }
            else
            {
                std::cerr
                    << "Accept error: "
                    << error.message()
                    << std::endl;
            }

            accept();
        }
    );
}