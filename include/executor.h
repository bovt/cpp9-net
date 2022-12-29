#pragma once

/// @file
/// @brief Файл с объявлением исполнителя блока команд

#include "bulk.h"
#include "logger.h"
#include <numeric>

/// @brief Класс исполнителя блока команд
class Executor
{
public:
    /// @brief Конструктор класса
    /// @param logger логгер объекта
    Executor(logging::Logger& logger) : logger_(logger) { }

    /// @brief Исполнить блок команд
    /// @param bulk блок команд
    void exec(Bulk& bulk) const
    {
        if (!bulk.empty())
        {
            logger_.write({serialize(bulk), bulk.time()});
        }
    }
private:
    std::string serialize(const Bulk& bulk) const
    {
        std::string s = std::accumulate(std::begin(bulk), std::end(bulk), std::string(),
            [](const std::string &ss, const std::string &s)
            {
                return ss.empty() ? s : ss + ", " + s;
            });
        return "bulk: " + s;
    }

    logging::Logger& logger_; ///< Логгер
};
