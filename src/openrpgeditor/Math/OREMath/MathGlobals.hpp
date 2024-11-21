#pragma once
#include <cmath>
#include <cfloat>
#include <cstdint>

enum class AspectRatioMode {
  Ignore,
  Keep,
  KeepByExpanding,
};

static bool fuzzyCompare(const double p1, const double p2) { return std::abs(p1 - p2) <= DBL_EPSILON; }

static bool fuzzyCompare(const float p1, const float p2) { return std::abs(p1 - p2) <= FLT_EPSILON; }

static bool fuzzyIsNull(const double p) { return std::abs(p) <= DBL_EPSILON; }

static bool fuzzyIsNull(const float p) { return std::abs(p) <= FLT_EPSILON; }

static bool isNull(const double d) {
  union U {
    double d;
    uint64_t u;
  };
  U val{};
  val.d = d;
  return (val.u & 0x7fffffffffffffffull) == 0;
}

static bool isNull(const float f) {
  union U {
    float f;
    uint32_t u;
  };
  U val{};
  val.f = f;
  return (val.u & 0x7fffffff) == 0;
}

template <typename T>
constexpr T oAbs(const T& t) {
  return t >= 0 ? t : -t;
}

constexpr int oRound(const double d) {
  return d >= 0.0 ? static_cast<int>(d + 0.5)
                  : static_cast<int>(d - static_cast<double>(static_cast<int>(d - 1)) + 0.5) + static_cast<int>(d - 1);
}
constexpr int oRound(const float d) {
  return d >= 0.0f ? static_cast<int>(d + 0.5f)
                   : static_cast<int>(d - static_cast<float>(static_cast<int>(d - 1)) + 0.5f) + static_cast<int>(d - 1);
}