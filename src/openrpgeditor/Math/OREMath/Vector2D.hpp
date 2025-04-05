#pragma once

#include "Point.hpp"

class Vector3D;
class Vector4D;
class Vector2D {
public:
  Vector2D();
  Vector2D(const float xpos, const float ypos)
  : m_x(xpos)
  , m_y(ypos) {}
  explicit Vector2D(const Point& point);
  explicit Vector2D(const PointF& point);
  explicit Vector2D(const Vector3D& vector);
  explicit Vector2D(const Vector4D& vector);

  [[nodiscard]] bool isNull() const { return ::isNull(m_x) && ::isNull(m_y); }

  [[nodiscard]] float x() const { return m_x; }
  [[nodiscard]] float y() const { return m_y; }
  void setX(const float x) { m_x = x; }
  void setY(const float y) { m_y = y; }
  [[nodiscard]] float length() const;
  [[nodiscard]] float lengthSquared() const;

  [[nodiscard]] Vector2D normalized() const;
  void normalize();

  Vector2D& operator+=(const Vector2D& vector) {
    m_x += vector.m_x;
    m_y += vector.m_y;
    return *this;
  }
  Vector2D& operator-=(const Vector2D& vector) {
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    return *this;
  }
  Vector2D& operator*=(float factor) {
    m_x *= factor;
    m_y *= factor;
    return *this;
  }
  Vector2D& operator*=(const Vector2D& vector) {
    m_x *= vector.m_x;
    m_y *= vector.m_y;
    return *this;
  }
  Vector2D& operator/=(float divisor) {
    m_x /= divisor;
    m_y /= divisor;
    return *this;
  }

  static float dotProduct(const Vector2D& v1, const Vector2D& v2);

  friend inline bool operator==(const Vector2D& v1, const Vector2D& v2);
  friend inline bool operator!=(const Vector2D& v1, const Vector2D& v2);
  friend inline Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
  friend inline Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
  friend inline Vector2D operator*(float factor, const Vector2D& vector);
  friend inline Vector2D operator*(const Vector2D& vector, float factor);
  friend inline Vector2D operator*(const Vector2D& v1, const Vector2D& v2);
  friend inline Vector2D operator-(const Vector2D& vector);
  friend inline Vector2D operator/(const Vector2D& vector, float divisor);

  friend inline bool fuzzyCompare(const Vector2D& v1, const Vector2D& v2);

  [[nodiscard]] Vector3D toVector3D() const;
  [[nodiscard]] Vector4D toVector4D() const;

  [[nodiscard]] Point toPoint() const { return {oRound(m_x), oRound(m_y)}; }
  [[nodiscard]] PointF toPointF() const { return {m_x, m_y}; }

private:
  float m_x;
  float m_y;

  friend class Vector3D;
  friend class Vector4D;
};

inline bool operator==(const Vector2D& v1, const Vector2D& v2) { return v1.m_x == v2.m_x && v1.m_y == v2.m_y; }

inline bool operator!=(const Vector2D& v1, const Vector2D& v2) { return v1.m_x != v2.m_x || v1.m_y != v2.m_y; }

inline Vector2D operator+(const Vector2D& v1, const Vector2D& v2) { return {v1.m_x + v2.m_x, v1.m_y + v2.m_y}; }

inline Vector2D operator-(const Vector2D& v1, const Vector2D& v2) { return {v1.m_x - v2.m_x, v1.m_y - v2.m_y}; }

inline Vector2D operator*(const float factor, const Vector2D& vector) { return {vector.m_x * factor, vector.m_y * factor}; }

inline Vector2D operator*(const Vector2D& vector, const float factor) { return {vector.m_x * factor, vector.m_y * factor}; }

inline Vector2D operator*(const Vector2D& v1, const Vector2D& v2) { return {v1.m_x * v2.m_x, v1.m_y * v2.m_y}; }

inline Vector2D operator-(const Vector2D& vector) { return {-vector.m_x, -vector.m_y}; }

inline Vector2D operator/(const Vector2D& vector, const float divisor) { return Vector2D(vector.m_x / divisor, vector.m_y / divisor); }

inline bool fuzzyCompare(const Vector2D& v1, const Vector2D& v2) { return fuzzyCompare(v1.m_x, v2.m_x) && fuzzyCompare(v1.m_y, v2.m_y); }