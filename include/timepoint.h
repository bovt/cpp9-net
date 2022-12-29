#pragma once

/// @file
/// @brief Файл с объявлением псевдонима типа времени

#include <chrono>

using TimePoint = std::chrono::system_clock::time_point;
