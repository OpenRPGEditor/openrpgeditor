#include "Database/RPGEquations.hpp"
#include <cmath>

namespace rpg::equations {
uint32_t experienceForLevel(int level, int basis, int extra, int accelA, int accelB) {
  double result = static_cast<double>(basis) * (pow(level - 1, 0.9 + static_cast<double>(accelA) / 250));
  result *= level * (level + 1) / (6 + pow(level, 2) / 50 / static_cast<double>(accelB));
  result += (level - 1) * extra;

  return round(result);
}
} // namespace rpg::equations