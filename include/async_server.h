#pragma once

/// @file
/// @brief Файл с объявлением асинхронного сервера

#include <boost/asio.hpp>

namespace async_server
{

namespace ba = boost::asio;

/// @brief Класс асинхронного сервера
class Server
{
public:
    /// @brief Конструктор
    /// @param io_context asio-контекст
    /// @param port порт, на котором будет запущен сервер
    Server(ba::io_context& io_context, std::uint16_t port) :
        acceptor_(io_context, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port))
    {
        do_accept();
    }

private:
    void do_accept();

    ba::ip::tcp::acceptor acceptor_;
};

} //namespace async_server
