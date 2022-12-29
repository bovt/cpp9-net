/// @file
/// @brief Файл с реализацией основного потока приложения

#include "async_server.h"
#include <boost/asio.hpp>
#include <iostream>

using namespace std::string_literals;

std::size_t n;

int main(int argc, char* argv[])
{
    try
    {
        std::uint16_t port;
        char* arg = argv[0];

        auto usage = "Usage: "s + arg + " " + "<port> <bulk size>";
        if (argc != 3)
        {
            std::cerr << usage << std::endl;
            return 1;
        }
        else try
        {
            arg = argv[1];
            port = std::stoi(arg);
            if (port < 1)
            {
                throw std::invalid_argument("port");
            }

            arg = argv[2];
            n = std::stoi(arg);
            if (n < 1)
            {
                throw std::invalid_argument("bulk size");
            }
        }
        catch (std::exception& e)
        {
            std::cerr << "Invalid argument: " << e.what() << '\n' << usage << std::endl;
            return 1;
        }

        async_server::ba::io_context io_context;
        async_server::Server server(io_context, port);
        io_context.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n";
    }
    return 0;
}
