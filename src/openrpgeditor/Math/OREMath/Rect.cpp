#include "OREMath/Rect.hpp"
#include <algorithm>

Rect Rect::normalized() const {
  Rect r;
  if (mRight < mLeft - 1) {
    r.mLeft = mRight;
    r.mRight = mLeft;
  } else {
    r.mLeft = mLeft;
    r.mRight = mRight;
  }
  if (mBottom < mTop - 1) {
    r.mTop = mBottom;
    r.mBottom = mTop;
  } else {
    r.mTop = mTop;
    r.mBottom = mBottom;
  }
  return r;
}

bool Rect::contains(const Point& p, const bool proper) const {
  int l, r;
  if (mRight < mLeft - 1) {
    l = mRight;
    r = mLeft;
  } else {
    l = mLeft;
    r = mRight;
  }
  if (proper) {
    if (p.x() <= l || p.x() >= r)
      return false;
  } else {
    if (p.x() < l || p.x() > r)
      return false;
  }
  int t, b;
  if (mBottom < mTop - 1) {
    t = mBottom;
    b = mTop;
  } else {
    t = mTop;
    b = mBottom;
  }
  if (proper) {
    if (p.y() <= t || p.y() >= b)
      return false;
  } else {
    if (p.y() < t || p.y() > b)
      return false;
  }
  return true;
}

bool Rect::contains(const Rect& r, const bool proper) const {
  if (isNull() || r.isNull())
    return false;

  int l1 = mLeft;
  int r1 = mLeft;
  if (mRight - mLeft + 1 < 0)
    l1 = mRight;
  else
    r1 = mRight;

  int l2 = r.mLeft;
  int r2 = r.mLeft;
  if (r.mRight - r.mLeft + 1 < 0)
    l2 = r.mRight;
  else
    r2 = r.mRight;

  if (proper) {
    if (l2 <= l1 || r2 >= r1)
      return false;
  } else {
    if (l2 < l1 || r2 > r1)
      return false;
  }

  int t1 = mTop;
  int b1 = mTop;
  if (mBottom - mTop + 1 < 0)
    t1 = mBottom;
  else
    b1 = mBottom;

  int t2 = r.mTop;
  int b2 = r.mTop;
  if (r.mBottom - r.mTop + 1 < 0)
    t2 = r.mBottom;
  else
    b2 = r.mBottom;

  if (proper) {
    if (t2 <= t1 || b2 >= b1)
      return false;
  } else {
    if (t2 < t1 || b2 > b1)
      return false;
  }

  return true;
}

Rect Rect::operator|(const Rect& r) const {
  if (isNull()) {
    return r;
  }

  if (r.isNull()) {
    return *this;
  }

  int l1 = mLeft;
  int r1 = mLeft;
  if (mRight - mLeft + 1 < 0) {
    l1 = mRight;
  } else {
    r1 = mRight;
  }

  int l2 = r.mLeft;
  int r2 = r.mLeft;
  if (r.mRight - r.mLeft + 1 < 0) {
    l2 = r.mRight;
  } else {
    r2 = r.mRight;
  }

  int t1 = mTop;
  int b1 = mTop;
  if (mBottom - mTop + 1 < 0) {
    l1 = mBottom;
  } else {
    r1 = mBottom;
  }

  int t2 = r.mTop;
  int b2 = r.mTop;
  if (r.mBottom - r.mTop + 1 < 0) {
    t2 = r.mBottom;
  } else {
    b2 = r.mBottom;
  }

  Rect tmp;
  tmp.mLeft = std::min(l1, l2);
  tmp.mRight = std::max(r1, r2);
  tmp.mTop = std::min(t1, t2);
  tmp.mBottom = std::max(b1, b2);
  return tmp;
}

Rect Rect::operator&(const Rect& r) const {
  if (isNull() || r.isNull()) {
    return {};
  }

  int l1 = mLeft;
  int r1 = mLeft;
  if (mRight - mLeft + 1 < 0) {
    l1 = mRight;
  } else {
    r1 = mRight;
  }

  int l2 = r.mLeft;
  int r2 = r.mLeft;
  if (r.mRight - r.mLeft + 1 < 0) {
    l2 = r.mRight;
  } else {
    r2 = r.mRight;
  }

  if (l1 > r2 || l2 > r1) {
    return {};
  }

  int t1 = mTop;
  int b1 = mTop;
  if (mBottom - mTop + 1 < 0) {
    t1 = mBottom;
  } else {
    b1 = mBottom;
  }

  int t2 = r.mTop;
  int b2 = r.mTop;
  if (r.mBottom - r.mTop + 1 < 0) {
    t2 = r.mBottom;
  } else {
    b2 = r.mBottom;
  }

  if (t1 > b2 || t2 > b1) {
    return {};
  }

  Rect tmp;
  tmp.mLeft = std::max(l1, l2);
  tmp.mRight = std::min(r1, r2);
  tmp.mTop = std::max(t1, t2);
  tmp.mBottom = std::min(b1, b2);
  return tmp;
}

bool Rect::intersects(const Rect& r) const {
  if (isNull() || r.isNull()) {
    return false;
  }

  int l1 = mLeft;
  int r1 = mLeft;
  if (mRight - mLeft + 1 < 0) {
    l1 = mRight;
  } else {
    r1 = mRight;
  }

  int l2 = r.mLeft;
  int r2 = r.mLeft;
  if (r.mRight - r.mLeft + 1 < 0) {
    l2 = r.mRight;
  } else {
    r2 = r.mRight;
  }

  if (l1 > r2 || l2 > r1) {
    return false;
  }

  int t1 = mTop;
  int b1 = mTop;
  if (mBottom - mTop + 1 < 0) {
    t1 = mBottom;
  } else {
    b1 = mBottom;
  }

  int t2 = r.mTop;
  int b2 = r.mTop;
  if (r.mBottom - r.mTop + 1 < 0) {
    t2 = r.mBottom;
  } else {
    b2 = r.mBottom;
  }

  if (t1 > b2 || t2 > b1) {
    return false;
  }

  return true;
}

RectF RectF::normalized() const {
  RectF ret = *this;
  if (ret.mWidth < 0.f) {
    ret.mX += ret.mWidth;
    ret.mWidth = -ret.mWidth;
  }

  if (ret.mHeight < 0.f) {
    ret.mY += ret.mHeight;
    ret.mHeight = -ret.mHeight;
  }

  return ret;
}

bool RectF::contains(const PointF& p) const {
  float l = mX;
  float r = mX;
  if (mWidth < 0.f) {
    l += mWidth;
  } else {
    r += mWidth;
  }

  if (l == r) {
    return false;
  }

  if (p.x() < l || p.x() > r) {
    return false;
  }

  float t = mY;
  float b = mY;

  if (mHeight < 0.f) {
    t += mHeight;
  } else {
    b += mHeight;
  }

  if (t == b) {
    return false;
  }

  if (p.y() < t || p.y() > b) {
    return false;
  }

  return true;
}

bool RectF::contains(const RectF& r) const {
  float l1 = mX;
  float r1 = mX;
  if (mWidth < 0.f) {
    l1 += mWidth;
  } else {
    r1 += mWidth;
  }

  if (l1 == r1) {
    return false;
  }

  float l2 = r.mX;
  float r2 = r.mX;
  if (r.mWidth < 0.f) {
    l2 += r.mWidth;
  } else {
    r2 += r.mWidth;
  }

  if (l2 == r2) {
    return false;
  }

  float t1 = mY;
  float b1 = mY;
  if (mHeight < 0.f) {
    t1 += mHeight;
  } else {
    b1 += mHeight;
  }

  if (t1 == b1) {
    return false;
  }

  float t2 = r.mY;
  float b2 = r.mY;
  if (r.mHeight < 0.f) {
    t2 += r.mHeight;
  } else {
    b2 += r.mHeight;
  }

  if (t2 == b2) {
    return false;
  }

  return true;
}

RectF RectF::operator|(const RectF& other) const {
  if (isNull()) {
    return {};
  }

  if (other.isNull()) {
    return {};
  }

  float left = mX;
  float right = mX;
  if (mWidth < 0.f) {
    left += mWidth;
  } else {
    right += mWidth;
  }

  if (other.mWidth < 0.f) {
    left = std::min(left, other.mX + other.mWidth);
    right = std::max(right, other.mX);
  } else {
    left = std::min(left, other.mX);
    right = std::max(right, other.mX + other.mWidth);
  }

  float top = mY;
  float bottom = mY;
  if (mHeight < 0.f) {
    top += mHeight;
  } else {
    bottom += mHeight;
  }

  if (other.mHeight < 0.f) {
    top = std::min(top, other.mY + other.mHeight);
    bottom = std::max(bottom, other.mY);
  } else {
    top = std::min(top, other.mY);
    bottom = std::max(bottom, other.mY + other.mHeight);
  }

  return RectF{left, top, right - left, bottom - top};
}

RectF RectF::operator&(const RectF& other) const {
  float l1 = mX;
  float r1 = mX;

  if (mWidth < 0) {
    l1 += mWidth;
  } else {
    r1 += mWidth;
  }

  if (l1 == r1) {
    return {};
  }

  float l2 = other.mX;
  float r2 = other.mX;

  if (other.mWidth < 0) {
    l2 += other.mWidth;
  } else {
    r2 += other.mWidth;
  }

  if (l2 == r2) {
    return {};
  }
  if (l1 >= r2 || l2 >= r1) {
    return {};
  }

  float t1 = mY;
  float b1 = mY;

  if (mHeight < 0) {
    t1 += mHeight;
  } else {
    b1 += mHeight;
  }

  if (t1 == b1) {
    return {};
  }

  float t2 = other.mY;
  float b2 = other.mY;

  if (other.mHeight < 0) {
    t2 += other.mHeight;
  } else {
    b2 += other.mHeight;
  }

  if (t2 == b2) {
    return {};
  }

  if (t1 >= b2 || t2 >= b1) {
    return {};
  }

  RectF tmp;
  tmp.mX = std::max(l1, l2);
  tmp.mY = std::max(t1, t2);
  tmp.mWidth = std::min(r1, r2) - tmp.mX;
  tmp.mHeight = std::min(b1, b2) - tmp.mY;
  return tmp;
}

bool RectF::intersects(const RectF& r) const {
  float l1 = mX;
  float r1 = mX;

  if (mWidth < 0) {
    l1 += mWidth;
  } else {
    r1 += mWidth;
  }

  if (l1 == r1) {
    return false;
  }

  float l2 = r.mX;
  float r2 = r.mX;

  if (r.mWidth < 0) {
    l2 += r.mWidth;
  } else {
    r2 += r.mWidth;
  }

  if (l2 == r2) {
    return false;
  }

  if (l1 >= r2 || l2 >= r1) {
    return false;
  }

  float t1 = mY;
  float b1 = mY;

  if (mHeight < 0) {
    t1 += mHeight;
  } else {
    b1 += mHeight;
  }

  if (t1 == b1) {
    return false;
  }

  float t2 = r.mY;
  float b2 = r.mY;

  if (r.mHeight < 0) {
    t2 += r.mHeight;
  } else {
    b2 += r.mHeight;
  }

  if (t2 == b2) {
    return false;
  }

  if (t1 >= b2 || t2 >= b1) {
    return false;
  }

  return true;
}

Rect RectF::toAlignedRect() const {
  const auto xMin = static_cast<int>(std::floor(mX));
  const auto xMax = static_cast<int>(std::ceil(mX + mWidth));
  const auto yMin = static_cast<int>(std::floor(mY));
  const auto yMax = static_cast<int>(std::ceil(mY + mHeight));

  return Rect{xMin, yMin, xMax - xMin, yMax - yMin};
}
