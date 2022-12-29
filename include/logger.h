#pragma once

/// @file
/// @brief Файл с объявлением логгера и приемников данных

#include "timepoint.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

namespace logging
{

/// @brief Структура сообщения логгера
struct Message
{
    std::string text_; ///< Текст сообщения
    TimePoint tp_ = TimePoint(); ///< Время сообщения
};

/// @brief Базовый класс приемника данных
class BaseSink
{
public:
    /// @brief Деструктор класса
    virtual ~BaseSink() = default;

    /// @brief Записать в приемник
    /// @param msg текст сообщения
    virtual void write(const Message& msg) = 0;
};

/// @brief Класс приемника данных в консоль
class CoutSink : public BaseSink
{
public:
    /// @brief Записать в приемник
    /// @param msg сообщение
    void write(const Message& msg) override
    {
        std::cout << msg.text_ << std::endl;
    }
};

/// @brief Класс приемника данных в файл
class FileSink : public BaseSink
{
public:
    /// @brief Записать в приемник
    /// @param msg сообщение
    void write(const Message& msg) override
    {
        auto t = std::chrono::system_clock::to_time_t(msg.tp_);
        auto fileName = "bulk" + std::to_string(t) + ".log";
        std::ofstream ofs(fileName, std::ios::out | std::ios::app);
        ofs << msg.text_ << std::endl;
    }
};

/// @brief Класс логгера
class Logger
{
public:
    /// @brief Записать в лог
    /// @param msg сообщение
    void write(const Message& msg)
    {
        for (const auto& sink : sinks_)
        {
            sink->write(msg);
        }
    }

    /// @brief Добавить приемник данных к логгеру
    /// @param sink приемник данных
    void addSink(std::unique_ptr<BaseSink> sink)
    {
        sinks_.push_back(std::move(sink));
    }
private:
    std::vector<std::unique_ptr<BaseSink>> sinks_; ///< Набор приемников данных
};

} //namespace logging
