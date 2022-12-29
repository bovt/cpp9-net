/// @file
/// @brief Файл с реализацией асинхронного сервера

#include "async_server.h"
#include "async_session.h"

using namespace async_server;

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, ba::ip::tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<Session>(std::move(socket))->start();
            }
            do_accept();
        });
}
