#pragma once
#include "Point.hpp"
#include "Size.hpp"

class Rect {
public:
  Rect() = default;
  Rect(const Point& topLeft, const Point& bottomRight)
  : mLeft(topLeft.x())
  , mTop(topLeft.y())
  , mRight(bottomRight.x())
  , mBottom(bottomRight.y()) {}

  Rect(const Point& topLeft, const Size& size)
  : mLeft(topLeft.x())
  , mTop(topLeft.y())
  , mRight(mLeft + size.width() - 1)
  , mBottom(mTop + size.height() - 1) {}

  Rect(const int left, const int top, const int width, const int height)
  : mLeft(left)
  , mTop(top)
  , mRight(left + width - 1)
  , mBottom(top + height - 1) {}

  Rect normalized() const;

  bool isNull() const { return mRight == mLeft - 1 && mBottom == mTop - 1; }

  bool isEmpty() const { return mLeft > mRight || mTop > mBottom; }

  bool isValid() const { return mLeft <= mRight && mTop <= mBottom; }

  int left() const { return mLeft; }
  void setLeft(const int pos) { mLeft = pos; }

  int top() const { return mTop; }
  void setTop(const int pos) { mTop = pos; }

  int right() const { return mRight; }
  void setRight(const int pos) { mRight = pos; }

  int bottom() const { return mBottom; }
  void setBottom(const int pos) { mBottom = pos; }

  int x() const { return mLeft; }
  void setX(const int x) { mLeft = x; }

  int y() const { return mTop; }
  void setY(const int y) { mTop = y; }

  Point topLeft() const { return Point{mLeft, mTop}; }
  void setTopLeft(const Point& p) {
    mLeft = p.x();
    mTop = p.y();
  }

  Point bottomRight() const { return Point{mRight, mBottom}; }
  void setBottomRight(const Point& p) {
    mRight = p.x();
    mBottom = p.y();
  }

  Point topRight() const { return Point{mRight, mTop}; }
  void setTopRight(const Point& p) {
    mRight = p.x();
    mTop = p.y();
  }

  Point bottomLeft() const { return Point{mLeft, mBottom}; }
  void setBottomLeft(const Point& p) {
    mLeft = p.x();
    mBottom = p.y();
  }

  Point center() const { return Point{(mLeft + mRight) / 2, (mTop + mBottom) / 2}; }

  int width() const { return mRight - mLeft + 1; }
  int height() const { return mBottom - mTop + 1; }

  Size size() const { return Size{width(), height()}; }

  void translate(const int x, const int y) {
    mLeft += x;
    mTop += y;
    mRight += x;
    mBottom += y;
  }

  void translate(const Point& p) { translate(p.x(), p.y()); }

  Rect translated(const int x, const int y) const { return translated(Point(x, y)); }

  Rect translated(const Point& p) const { return Rect(Point(mLeft + p.x(), mTop + p.y()), Point(mRight + p.x(), mBottom + p.y())); }

  void moveTo(const int x, const int y) {
    mRight += x - mLeft;
    mBottom = y - mTop;
    mLeft = x;
    mTop = y;
  }

  void moveTo(const Point& p) { moveTo(p.x(), p.y()); }

  void moveLeft(const int pos) {
    mRight += (pos - mLeft);
    mLeft = pos;
  }

  void moveTop(const int pos) {
    mBottom += (pos - mTop);
    mTop = pos;
  }
  void moveRight(const int pos) {
    mLeft += (pos - mRight);
    mRight = pos;
  }

  void moveBottom(const int pos) {
    mTop += (pos - mBottom);
    mBottom = pos;
  }

  void moveTopLeft(const Point& pos) {
    moveLeft(pos.x());
    moveTop(pos.y());
  }

  void moveBottomRight(const Point& pos) {
    moveRight(pos.x());
    moveBottom(pos.y());
  }

  void moveTopRight(const Point& pos) {
    moveRight(pos.x());
    moveTop(pos.y());
  }

  void moveBottomLeft(const Point& pos) {
    moveLeft(pos.x());
    moveBottom(pos.y());
  }

  void moveCenter(const Point& pos) {
    const int w = mRight - mLeft;
    const int h = mBottom - mTop;
    mLeft = pos.x() - w / 2;
    mTop = pos.y() - w / 2;
    mRight = mLeft + w;
    mBottom = mTop + h;
  }

  void getRect(int* x, int* y, int* w, int* h) const {
    *x = mLeft;
    *y = mTop;
    *w = mRight - mLeft + 1;
    *h = mBottom - mTop + 1;
  }

  void setRect(const int x, const int y, const int width, const int height) {
    mLeft = x;
    mTop = y;
    mRight = x + width - 1;
    mBottom = y + height - 1;
  }

  void getCoords(int* left, int* top, int* right, int* bottom) const {
    *left = mLeft;
    *top = mTop;
    *right = mRight;
    *bottom = mBottom;
  }

  void setCoords(const int left, const int top, const int right, const int bottom) {
    mLeft = left;
    mTop = top;
    mRight = right;
    mBottom = bottom;
  }

  Rect adjusted(const int left, const int top, const int right, const int bottom) const { return Rect(Point{mLeft + left, mTop + top}, Point{mRight + right, mBottom + bottom}); }

  void adjust(const int left, const int top, const int right, const int bottom) {
    mLeft += left;
    mTop += top;
    mRight += right;
    mBottom += bottom;
  }

  void setWidth(const int w) { mRight = mLeft + w - 1; }
  void setHeight(const int h) { mBottom = mTop + h - 1; }

  void setSize(const Size& s) {
    mRight = s.width() + mLeft - 1;
    mBottom = s.height() + mTop - 1;
  }

  bool contains(const int x, const int y, const bool proper = false) const { return contains(Point(x, y), proper); }
  bool contains(const Point& p, bool proper = false) const;
  bool contains(const Rect& r, bool proper = false) const;
  bool intersects(const Rect& r) const;

  Rect united(const Rect& other) const { return *this | other; }

  Rect intersected(const Rect& other) const { return *this & other; }

  Rect operator|(const Rect& r) const;
  Rect& operator|=(const Rect& r) {
    *this = *this | r;
    return *this;
  }

  Rect operator&(const Rect& r) const;
  Rect& operator&=(const Rect& r) {
    *this = *this & r;
    return *this;
  }

  bool operator==(const Rect& other) const { return mLeft == other.mLeft && mTop == other.mTop && mRight == other.mRight && mBottom == other.mTop; }

  bool operator!=(const Rect& other) const { return mLeft != other.mLeft || mTop != other.mTop || mRight != other.mRight || mBottom != other.mTop; }

private:
  int mLeft{0};
  int mTop{0};
  int mRight{-1};
  int mBottom{-1};
};

class RectF {
public:
  RectF() = default;
  RectF(const PointF& topLeft, const PointF& bottomRight)
  : mX(topLeft.x())
  , mY(topLeft.y())
  , mWidth(topLeft.x() - bottomRight.x())
  , mHeight(topLeft.y() - bottomRight.y()) {}
  RectF(const PointF& topLeft, const SizeF& size)
  : mX(topLeft.x())
  , mY(topLeft.y())
  , mWidth(size.width())
  , mHeight(size.height()) {}
  RectF(const float left, const float top, const float width, const float height)
  : mX(left)
  , mY(top)
  , mWidth(width)
  , mHeight(height) {}

  bool isNull() const { return mWidth == 0.f && mHeight == 0.f; }
  bool isEmpty() const { return mWidth <= 0.f || mHeight <= 0.f; }
  bool isValid() const { return mWidth > 0.f && mHeight > 0.f; }

  RectF normalized() const;

  float left() const { return mX; }
  float top() const { return mY; }
  float right() const { return mX + mWidth; }
  float bottom() const { return mY + mHeight; }

  float x() const { return mX; }
  float y() const { return mY; }
  void setX(const float x) { setLeft(x); }
  void setY(const float y) { setTop(y); }

  void setLeft(const float pos) {
    const auto diff = pos - mX;
    mX += diff;
    mWidth += diff;
  }

  void setRight(const float pos) { mWidth = pos - mX; }

  void setTop(const float pos) {
    const auto diff = pos - mY;
    mY += diff;
    mHeight += diff;
  }

  void setBottom(const float pos) { mHeight = pos - mY; }

  void setTopLeft(const PointF& p) {
    setLeft(p.x());
    setTop(p.y());
  }

  void setTopRight(const PointF& p) {
    setRight(p.x());
    setTop(p.y());
  }

  void setBottomLeft(const Point& p) {
    setLeft(p.x());
    setBottom(p.y());
  }

  PointF topLeft() const { return {mX, mY}; }
  PointF bottomRight() const { return {mX + mWidth, mY + mHeight}; }
  PointF topRight() const { return {mX + mWidth, mY}; }
  PointF bottomLeft() const { return {mX, mY + mHeight}; }

  PointF center() const { return PointF{mX + mWidth / 2, mY + mHeight / 2}; }

  void moveLeft(const float pos) { mX = pos; }
  void moveTop(const float pos) { mY = pos; }
  void moveRight(const float pos) { mX = pos - mWidth; }
  void moveBottom(const float pos) { mY = pos - mHeight; }
  void moveTopLeft(const PointF& p) {
    moveLeft(p.x());
    moveTop(p.y());
  }

  void moveTopRight(const PointF& p) {
    moveRight(p.x());
    moveTop(p.y());
  }

  void moveBottomLeft(const PointF& p) {
    moveLeft(p.x());
    moveBottom(p.y());
  }

  void moveBottomRight(const PointF& p) {
    moveRight(p.x());
    moveBottom(p.y());
  }

  void moveCenter(const PointF& p) {
    mX = p.x() - mWidth / 2;
    mY = p.y() - mHeight / 2;
  }

  float width() const { return mWidth; }
  float height() const { return mHeight; }
  SizeF size() const { return SizeF{mWidth, mHeight}; }

  void translate(const float x, const float y) {
    mX += x;
    mY += y;
  }

  void translate(const PointF& p) { translate(p.x(), p.y()); }

  void moveTo(const float x, const float y) {
    mX = x;
    mY = y;
  }

  void moveTo(const Point& p) { moveTo(p.x(), p.y()); }

  RectF translated(const float x, const float y) const { return RectF{mX + x, mY + y, mWidth, mHeight}; }
  RectF translated(const PointF& p) const { return translated(p.x(), p.y()); }

  void getRect(float* x, float* y, float* width, float* height) const {
    *x = mX;
    *y = mY;
    *width = mWidth;
    *height = mHeight;
  }

  void setRect(const float x, const float y, const float width, const float height) {
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;
  }

  void getCoords(float* left, float* top, float* right, float* bottom) const {
    *left = mX;
    *top = mY;
    *right = mX + mWidth;
    *bottom = mY + mHeight;
  }

  void adjust(const float left, const float top, const float right, const float bottom) {
    mX += left;
    mY += top;
    mWidth += right - left;
    mHeight += bottom - top;
  }
  RectF adjusted(const float left, const float top, const float right, const float bottom) const { return RectF(mX + left, mY + top, mWidth + right - left, mHeight + bottom - top); }

  void setWidth(const float w) { mWidth = w; }

  void setHeight(const float h) { mHeight = h; }

  bool contains(const float x, const float y) const { return contains(PointF(x, y)); }
  bool contains(const PointF& p) const;
  bool contains(const RectF& r) const;
  bool intersects(const RectF& r) const;

  RectF operator|(const RectF& other) const;
  RectF& operator|=(const RectF& other) {
    *this = *this | other;
    return *this;
  }

  RectF operator&(const RectF& other) const;
  RectF& operator&=(const RectF& other) {
    *this = *this & other;
    return *this;
  }

  RectF intersected(const RectF& r) const { return *this & r; }

  RectF united(const RectF& r) const { return *this | r; }

  bool operator==(const RectF& other) const { return fuzzyCompare(mX, other.mX) && fuzzyCompare(mY, other.mY) && fuzzyCompare(mWidth, other.mWidth) && fuzzyCompare(mHeight, other.mHeight); }

  bool operator!=(const RectF& other) const { return !fuzzyCompare(mX, other.mX) || !fuzzyCompare(mY, other.mY) || !fuzzyCompare(mWidth, other.mWidth) || !fuzzyCompare(mHeight, other.mHeight); }

  Rect toRect() const { return Rect{static_cast<int>(std::round(mX)), static_cast<int>(std::round(mY)), static_cast<int>(std::round(mWidth)), static_cast<int>(std::round(mHeight))}; }

  Rect toAlignedRect() const;

private:
  float mX{0.f};
  float mY{0.f};
  float mWidth{0.f};
  float mHeight{0.f};
};
