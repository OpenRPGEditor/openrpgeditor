#pragma once
#include "MathGlobals.hpp"
#include "imgui.h"

#include <algorithm>
#include <cmath>

class Point {
public:
  Point() = default;
  explicit Point(const int x, const int y) : mX(x), mY(y) {}
  bool isNull() const { return mX == 0 && mY == 0; }

  explicit operator ImVec2() const { return ImVec2{static_cast<float>(mX), static_cast<float>(mY)}; }

  int manhattanLength() const { return std::abs(mX) + std::abs(mY); }

  int x() const { return mX; }
  int& xr() { return mX; }
  int y() const { return mY; }
  int& yr() { return mY; }

  Point& operator+=(const Point& p) {
    mX += p.x();
    mY += p.y();
    return *this;
  }

  Point& operator-=(const Point& p) {
    mX -= p.x();
    mY -= p.y();
    return *this;
  }

  Point& operator*=(const float c) {
    mX = std::round(mX * c);
    mY = std::round(mY * c);
    return *this;
  }

  Point& operator/=(const float c) {
    mX = std::round(mX / c);
    mY = std::round(mY / c);
    return *this;
  }

  friend inline bool operator==(const Point&, const Point&);
  friend inline bool operator!=(const Point&, const Point&);
  friend inline Point operator+(const Point&, const Point&);
  friend inline Point operator-(const Point&, const Point&);
  friend inline Point operator*(const Point&, float);
  friend inline Point operator*(float, const Point&);
  friend inline Point operator-(const Point&);
  friend inline Point operator/(const Point&, float);

private:
  int mX{0};
  int mY{0};
};

inline bool operator==(const Point& lhs, const Point& rhs) { return lhs.mX == rhs.mX && lhs.mY == rhs.mY; }
inline bool operator!=(const Point& lhs, const Point& rhs) { return lhs.mX != rhs.mX || lhs.mY != rhs.mY; }

inline Point operator+(const Point& lhs, const Point& rhs) { return Point{lhs.mX + rhs.mX, lhs.mY + rhs.mY}; }
inline Point operator-(const Point& lhs, const Point& rhs) { return Point{lhs.mX - rhs.mX, lhs.mY - rhs.mY}; }
inline Point operator*(const Point& lhs, const float rhs) {
  return Point{static_cast<int>(std::round(lhs.mX * rhs)), static_cast<int>(std::round(lhs.mY * rhs))};
}
inline Point operator*(const float lhs, const Point& rhs) {
  return Point{static_cast<int>(std::round(lhs * rhs.mX)), static_cast<int>(std::round(lhs * rhs.mY))};
}

inline Point operator-(const Point& lhs) { return Point{-lhs.mX, -lhs.mY}; }

inline Point operator/(const Point& lhs, const float rhs) {
  return Point{static_cast<int>(std::round(lhs.mX / rhs)), static_cast<int>(std::round(lhs.mY / rhs))};
}

class PointF {
public:
  PointF() = default;
  PointF(const float x, const float y) : mX(x), mY(y) {}
  bool isNull() const { return mX == 0.f && mY == 0.f; }
  int manhattanLength() const { return std::abs(mX) + std::abs(mY); }

#ifdef IMGUI_VERSION
  operator ImVec2() const { return ImVec2{mX, mY}; }
#endif

  float x() const { return mX; }
  float& xr() { return mX; }
  float y() const { return mY; }
  float& yr() { return mY; }

  PointF& operator+=(const PointF& p) {
    mX += p.x();
    mY += p.y();
    return *this;
  }

  PointF& operator-=(const PointF& p) {
    mX -= p.x();
    mY -= p.y();
    return *this;
  }

  PointF& operator*=(const float c) {
    mX *= c;
    mY *= c;
    return *this;
  }

  PointF& operator/=(const float c) {
    mX /= c;
    mY /= c;
    return *this;
  }

  friend inline bool operator==(const PointF&, const PointF&);
  friend inline bool operator!=(const PointF&, const PointF&);
  friend inline PointF operator+(const PointF&, const PointF&);
  friend inline PointF operator-(const PointF&, const PointF&);
  friend inline PointF operator*(const PointF&, float);
  friend inline PointF operator*(float, const PointF&);
  friend inline PointF operator-(const PointF&);
  friend inline PointF operator/(const PointF&, float);

private:
  float mX{0.f};
  float mY{0.f};
};

inline bool operator==(const PointF& lhs, const PointF& rhs) {
  return fuzzyIsNull(lhs.mX - rhs.mX) && fuzzyIsNull(lhs.mY - rhs.mY);
}
inline bool operator!=(const PointF& lhs, const PointF& rhs) {
  return !fuzzyIsNull(lhs.mX - rhs.mX) || !fuzzyIsNull(lhs.mY - rhs.mY);
}

inline PointF operator+(const PointF& lhs, const PointF& rhs) { return PointF{lhs.mX + rhs.mX, lhs.mY + rhs.mY}; }
inline PointF operator-(const PointF& lhs, const PointF& rhs) { return PointF{lhs.mX - rhs.mX, lhs.mY - rhs.mY}; }
inline PointF operator*(const PointF& lhs, const float rhs) { return PointF{lhs.mX * rhs, lhs.mY * rhs}; }
inline PointF operator*(const float lhs, const PointF& rhs) { return PointF{lhs * rhs.mX, lhs * rhs.mY}; }
inline PointF operator-(const PointF& lhs) { return PointF{-lhs.mX, -lhs.mY}; }
inline PointF operator/(const PointF& lhs, const float rhs) { return PointF{lhs.mX / rhs, lhs.mY / rhs}; }
