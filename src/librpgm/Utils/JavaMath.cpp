#include "Utils/JavaMath.hpp"

#include <cassert>
#include <chrono>
#include <ctime>
#include <random>

namespace Math {
static auto gen = std::mt19937(std::time(nullptr) & 0x7FFFFFFF);
static bool use2k = false;

void use2kRandom(bool use) { use2k = use; }

double random() {
  auto ret = randomInt(0x7FFFFFFF) / static_cast<double>(0x7FFFFFFF);
  return ret;
}

uint32_t randomInt(const uint32_t max) {
  if (!use2k) {
    return std::uniform_int_distribution<uint32_t>(0, max - 1)(gen);
  }

  if (max == 0xFFFFFFFF) {
    return gen() & 0x7FFFFFFF;
  }

  uint32_t rem = -max % max;
  while (true) {
    uint32_t n = static_cast<uint32_t>(gen());
    if (n >= rem) {
      return n % max;
    }
  }
}
double mod(const double x, const int y) { return fmod(fmod(x, y) + y, y); }

void seed(const int seed) { gen = std::mt19937(seed); }

} // namespace Math