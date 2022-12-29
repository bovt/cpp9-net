#pragma once

/// @file
/// @brief Файл с объявлением класса потока исполнения

#include <chrono>
#include <functional>
#include <future>
#include <thread>

using namespace std::chrono_literals;

/// @brief Класс потока исполнения
class Thread
{
public:
    /// @brief Конструктор
    /// @param name имя потока
    Thread(const std::string& name) : name_(name) { }

    /// @brief Конструктор
    /// @param name имя потока
    /// @param f функция исполнения
    Thread(const std::string& name, std::function<void()> f) : name_(name), f_(std::move(f)) { }

    /// @brief Деструктор
    ~Thread()
    {
        waitStop();
    }

    /// @brief Запустить поток
    void start()
    {
        if (t_.joinable()) // на случай перезапуска потока, который еще не завершился из-за команды асинхронной остановки
        {
            t_.join();
        }
        if (!f_)
        {
            throw std::bad_function_call();
        }
        t_ = std::thread(f_);
    }

    /// @brief Запустить поток
    /// @param f функция исполнения
    void start(std::function<void()> f)
    {
        f_ = std::move(f);
        start();
    }

    /// @brief Остановить поток
    void waitStop()
    {
        if (t_.joinable())
        {
            t_.join();
        }
    }

    /// @brief Остановить поток
    /// @tparam Func тип функции остановки
    /// @param func функция остановки потока
    /// @param sync остановить, дожидаясь реальной остановки или нет
    template<typename Func>
    void stop(Func&& func, bool sync = false)
    {
        func();
        if (sync)
        {
            waitStop();
        }
    }

    /// @brief Проверить запущен ли поток
    /// @return true, если запущен или false, если нет
    bool isStarted() const
    {
        return t_.joinable();
    }

    /// @brief Получить имя потока
    /// @return имя потока
    std::string name() const
    {
        return name_;
    }
protected:
    std::string name_; ///< Имя потока
    std::function<void()> f_; ///< Функция исполнения потока
    std::thread t_; /// Объект потока
};
