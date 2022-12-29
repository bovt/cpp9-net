#pragma once

/// @file
/// @brief Файл с объявлением класса блока команд

#include "timepoint.h"
#include <string>
#include <vector>

/// @brief Класс блока команд
class Bulk : private std::vector<std::string>
{
    using vector = std::vector<std::string>;
public:
    using vector::empty;
    using vector::size;
    using vector::clear;
    using vector::begin;
    using vector::end;

    /// @brief Добавить команду в блок
    /// @tparam T тип значения команды
    /// @param str значение команды
    template<class T>
    void push_back(T&& str)
    {
        if (empty())
        {
            tp_ = std::chrono::system_clock::now();
        }
        vector::emplace_back(std::forward<T>(str));
    }

    /// @brief Получить время записи первой команды в блок
    /// @return время записи первой команды в блок
    auto time() const { return tp_; }
private:
    TimePoint tp_; ///< Время приема первой команды
};
