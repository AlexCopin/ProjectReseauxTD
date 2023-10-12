#pragma once

#include <cstdint>

constexpr std::uint16_t AppPort = 14769;

constexpr std::uint32_t TickRate = 50; //< Correspond au timestep de 0.02 côté Unity
constexpr std::uint32_t TickDelay = 1000 / TickRate;
