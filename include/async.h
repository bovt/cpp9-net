#pragma once

/// @file
/// @brief Файл с объявлением интерфейса исполнителя блока команд

#include <cstddef>

namespace async {

using handle_t = std::size_t;

/// @brief Подключиться к исполнителю блоков команд
/// @param bulk размер блока команд
/// @return контекст
handle_t connect(std::size_t bulk);

/// @brief Передать строковые данные
/// @param handle контекст
/// @param data указатель на буфер данных
/// @param size размер буфера
void receive(handle_t handle, const char *data, std::size_t size);

/// @brief Отключиться от исполнителя
/// @param handle контекст
void disconnect(handle_t handle);

}
