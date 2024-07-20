#pragma once
#include "imgui.h"
#include "OREMath/MathGlobals.hpp"

class Size {
public:
  Size() = default;
  Size(const int width, const int height) : mWidth(width), mHeight(height) {}

  explicit operator ImVec2() const { return ImVec2{static_cast<float>(mWidth), static_cast<float>(mHeight)}; }

  void transpose();

  bool isNull() const { return mWidth == 0 && mHeight == 0; }
  bool isEmpty() const { return mWidth < 1 || mHeight < 1; }
  bool isValid() const { return mWidth >= 0 && mHeight >= 0; }

  int width() const { return mWidth; }
  int& rwidth() { return mWidth; }
  void setWidth(const int width) { mWidth = width; }

  int height() const { return mHeight; }
  int& rheight() { return mHeight; }
  void setHeight(const int height) { mHeight = height; }

  void scale(const int w, const int h, const AspectRatioMode mode) { scale(Size{w, h}, mode); }
  void scale(const Size& size, AspectRatioMode mode);

  Size& operator+=(const Size& s) {
    mWidth += s.mWidth;
    mHeight += s.mHeight;
    return *this;
  }
  Size& operator-=(const Size& s) {
    mWidth -= s.mWidth;
    mHeight -= s.mHeight;
    return *this;
  }

  Size& operator*=(const Size& s) {
    mWidth *= s.mWidth;
    mHeight *= s.mHeight;
    return *this;
  }

  bool operator==(const Size& rhs) const { return mWidth == rhs.mWidth && mHeight == rhs.mHeight; }
  bool operator!=(const Size& rhs) const { return mWidth != rhs.mWidth || mHeight != rhs.mHeight; }

  friend inline Size operator+(const Size&, const Size&);
  friend inline Size operator-(const Size&, const Size&);
  friend inline Size operator*(const Size&, float);
  friend inline Size operator*(float, const Size&);
  friend inline Size operator/(const Size&, float);

private:
  int mWidth{-1};
  int mHeight{-1};
};

Size operator+(const Size& lhs, const Size& rhs) { return Size{lhs.mWidth + rhs.mWidth, lhs.mHeight + rhs.mHeight}; }
Size operator-(const Size& lhs, const Size& rhs) { return Size{lhs.mWidth - rhs.mWidth, lhs.mHeight - rhs.mHeight}; }
Size operator*(const Size& lhs, const float rhs) {
  return Size{static_cast<int>(lhs.mWidth * rhs), static_cast<int>(lhs.mHeight * rhs)};
}

Size operator/(const Size& lhs, const float rhs) {
  return Size{static_cast<int>(lhs.mWidth / rhs), static_cast<int>(lhs.mHeight / rhs)};
}

class SizeF {
public:
  SizeF() = default;
  SizeF(const float width, const float height) : mWidth(width), mHeight(height) {}
  explicit operator ImVec2() const { return ImVec2{mWidth, mHeight}; }

  void transpose();

  bool isNull() const { return mWidth == 0 && mHeight == 0; }
  bool isEmpty() const { return mWidth < 1 || mHeight < 1; }
  bool isValid() const { return mWidth >= 0 && mHeight >= 0; }

  float width() const { return mWidth; }
  float& rwidth() { return mWidth; }
  void setWidth(const int width) { mWidth = width; }

  float height() const { return mHeight; }
  float& rheight() { return mHeight; }
  void setHeight(const int height) { mHeight = height; }

  void scale(const float w, const float h, const AspectRatioMode mode) { scale(SizeF{w, h}, mode); }
  void scale(const SizeF& size, AspectRatioMode mode);

  SizeF& operator+=(const SizeF& s) {
    mWidth += s.mWidth;
    mHeight += s.mHeight;
    return *this;
  }
  SizeF& operator-=(const SizeF& s) {
    mWidth -= s.mWidth;
    mHeight -= s.mHeight;
    return *this;
  }

  SizeF& operator*=(const SizeF& s) {
    mWidth *= s.mWidth;
    mHeight *= s.mHeight;
    return *this;
  }

  friend bool operator==(const SizeF& lhs, const SizeF& rhs);
  friend bool operator!=(const SizeF& lhs, const SizeF& rhs);

  friend inline SizeF operator+(const SizeF&, const SizeF&);
  friend inline SizeF operator-(const SizeF&, const SizeF&);
  friend inline SizeF operator*(const SizeF&, float);
  friend inline SizeF operator*(float, const SizeF&);
  friend inline SizeF operator/(const SizeF&, float);

private:
  float mWidth{-1};
  float mHeight{-1};
};

inline bool operator==(const SizeF& lhs, const SizeF& rhs) {
  return lhs.mWidth == rhs.mWidth && lhs.mHeight == rhs.mHeight;
}

inline bool operator!=(const SizeF& lhs, const SizeF& rhs) {
  return lhs.mWidth != rhs.mWidth || lhs.mHeight != rhs.mHeight;
}

SizeF operator+(const SizeF& lhs, const SizeF& rhs) {
  return SizeF{lhs.mWidth + rhs.mWidth, lhs.mHeight + rhs.mHeight};
}
SizeF operator-(const SizeF& lhs, const SizeF& rhs) {
  return SizeF{lhs.mWidth - rhs.mWidth, lhs.mHeight - rhs.mHeight};
}
SizeF operator*(const SizeF& lhs, const float rhs) { return SizeF{lhs.mWidth * rhs, lhs.mHeight * rhs}; }
SizeF operator*(const float lhs, const SizeF& rhs) { return SizeF{lhs * rhs.mWidth, lhs * rhs.mHeight}; }

SizeF operator/(const SizeF& lhs, const float rhs) { return SizeF{lhs.mWidth / rhs, lhs.mHeight / rhs}; }
