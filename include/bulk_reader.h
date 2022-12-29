#pragma once

/// @file
/// @brief Файл с объявлением класса читателя блока команд

#include <memory>
#include <string>

class Bulk;

/// @brief Класс читателя блока команд
class BulkReader
{
public:
    /// @brief Состояние читателя блока команд
    enum State
    {
        OPENED_BULK, ///< Открытый блок
        CLOSED_BULK  ///< Закрытый блок
    };

    /// @brief Конструктор класса
    /// @param is входящий поток данных
    /// @param n размер блока
    BulkReader(std::istream& is, std::size_t n) : is_(is), n_(n) { }

    /// @brief Прочитать блок команд
    /// @param bulk блок команд
    /// @return есть ли еще команды для чтения
    bool read(Bulk& bulk);

    /// @brief Получить состояние читателя блока команд
    /// @return состояние читателя блока команд
    State state() const;
private:
    void ltrim(std::string& str) const;

    std::istream& is_;
    const std::size_t n_ = 0;
    size_t openDepth_ = 0;
    std::string buffer_;
};
