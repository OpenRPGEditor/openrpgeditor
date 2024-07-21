#pragma once
#include <cmath>
#include <cfloat>
#include <cstdint>

enum class AspectRatioMode {
  Ignore,
  Keep,
  KeepByExpanding,
};

static inline bool fuzzyCompare(const double p1, const double p2) { return std::abs(p1 - p2) <= DBL_EPSILON; }

static inline bool fuzzyCompare(const float p1, const float p2) { return std::abs(p1 - p2) <= FLT_EPSILON; }

static inline bool fuzzyIsNull(const double p) { return std::abs(p) <= DBL_EPSILON; }

static inline bool fuzzyIsNull(const float p) { return std::abs(p) <= FLT_EPSILON; }

static inline bool isNull(const double d) {
  union U {
    double d;
    uint64_t u;
  };
  U val;
  val.d = d;
  return val.u == 0ull;
}

static inline bool isNull(const float f) {
  union U {
    float f;
    uint32_t u;
  };
  U val;
  val.f = f;
  return val.u == 0u;
}