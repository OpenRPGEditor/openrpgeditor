#pragma once
#include <cstdint>

namespace Math {
double random();
uint32_t randomInt(uint32_t max);
double mod(double x, int y);
void seed(int seed);
void use2kRandom(bool use);
} // namespace Math