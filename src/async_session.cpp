/// @file
/// @brief Файл с реализацией асинхронной сессии пользователя

#include "async_session.h"
#include "bulk.h"
#include <iostream>
#include <memory>

using namespace async_server;

void Session::do_read()
{
    socket_.async_read_some(ba::buffer(data_, max_length_),
        [this, self = shared_from_this()](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                is_ << std::string(data_, length);

                while (reader_.read(bulk_))
                {
                    if (state_ == BulkReader::OPENED_BULK)
                    {
                        std::string cmds;
                        for (auto& cmd : bulk_)
                        {
                            cmds.append(cmd + '\n');
                        }
                        bulk_.clear();
                        async::receive(handle_, cmds.c_str(), cmds.size());
                        state_ = BulkReader::CLOSED_BULK;
                    }
                    else
                    {
                        for (auto& cmd : bulk_)
                        {
                            async::receive(handle_, cmd.c_str(), cmd.size());
                        }
                        bulk_.clear();
                    }
                }
                is_.clear();
                if (reader_.state() == BulkReader::CLOSED_BULK)
                {
                    for (auto& cmd : bulk_)
                    {
                        async::receive(handle_, cmd.c_str(), cmd.size());
                    }
                    bulk_.clear();
                }
                else
                {
                    if (state_ == BulkReader::CLOSED_BULK)
                    {
                        async::receive(handle_, "", 0);
                        state_ = BulkReader::OPENED_BULK;
                    }
                }
                do_read();
            }
            else
            {
                async::disconnect(handle_);
            }
        });
}
