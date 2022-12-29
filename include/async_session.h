#pragma once

/// @file
/// @brief Файл с объявлением асинхронной сессии пользователя

#include "async.h"
#include "bulk.h"
#include "bulk_reader.h"
#include <boost/asio.hpp>
#include <sstream>

extern std::size_t n; ///< размер блока команд

namespace async_server
{

namespace ba = boost::asio;

/// @brief Класс асинхронной сессии
class Session : public std::enable_shared_from_this<Session>
{
public:
    /// @brief Конструктор
    /// @param socket клиентский сокет
    Session(ba::ip::tcp::socket socket) :
        socket_(std::move(socket)),
        reader_(is_, n)
    {
        handle_ = async::connect(n);
    }

    /// @brief Начать чтение и обработку данных
    void start()
    {
        do_read();
    }

private:
    void do_read();

    ba::ip::tcp::socket socket_;
    static constexpr std::size_t max_length_ = 1024;
    char data_[max_length_];

    async::handle_t handle_;
    Bulk bulk_;
    std::stringstream is_;
    BulkReader reader_;
    BulkReader::State state_ = BulkReader::CLOSED_BULK;
};

} //namespace async_server
