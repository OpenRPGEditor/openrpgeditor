#pragma once
#include <cinttypes>

#include <array>
namespace rpg::equations {
uint32_t experienceForLevel(int level, int basis, int extra, int accelA, int accelB);
void generateParameters(int value1, int value2, int growth, std::array<int, 99>& parameters);
void quickGenerateParameters(int rank, int maxValue, std::array<int, 99>& parameters);
} // namespace rpg::equations