#include "Utils/JavaMath.hpp"
#include <random>

namespace Math {
double random() {
  static std::random_device rd;
  std::mt19937 gen{rd()};
  return std::uniform_real_distribution(0.0, 1.0)(gen);
}

int randomInt(const int max) { return std::floor(max * random()); }
double mod(const double x, const int y) { return fmod(fmod(x, y) + y, y); }

} // namespace Math