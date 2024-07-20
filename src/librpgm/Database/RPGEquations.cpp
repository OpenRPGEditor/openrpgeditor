#include "Database/RPGEquations.hpp"
#include <cmath>

namespace rpg::equations {
uint32_t experienceForLevel(const int level, const int basis, const int extra, const int accelA, const int accelB) {
  double result = static_cast<double>(basis) * (pow(level - 1, 0.9 + static_cast<double>(accelA) / 250));
  result *= level * (level + 1) / (6 + pow(level, 2) / 50 / static_cast<double>(accelB));
  result += (level - 1) * extra;

  return round(result);
}
void generateParameters(const int value1, const int value2, const int growth, std::array<int, 99>& parameters) {
  const double a = value1;
  const double b = value2;
  const double c = growth - 10;

  for (int i = 1; i <= 99; i++) {
    const double n1 = a + ((b - a) * (i - 1) / 98);
    const double n2 = a + ((b - a) * (i - 1) * (i - 1) / 98 / 98);
    const double x = ceil((n2 * c + n1 * (10 - c)) / 10);
    parameters[i] = static_cast<int>(x);
  }
}

void quickGenerateParameters(const int rank, const int paramId, const int maxValue, std::array<int, 99>& parameters) {
  int value1 = 200 + rank * 100 + rand() * 100;
  int value2 = 4000 + rank * 1000 + rand() * 1000;

  value1 *= maxValue / 10000;
  value2 *= maxValue / 10000;

  if (paramId >= 2) {
    value1 += 5;
  }

  value1 = round(value1);
  value2 = round(value2);
  generateParameters(value1, value2, 10, parameters);
}

} // namespace rpg::equations