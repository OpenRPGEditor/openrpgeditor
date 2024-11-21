#pragma once

#include "Point.hpp"

class Matrix4x4;
class Vector2D;
class Vector4D;
class Vector3D {
public:
  Vector3D();
  Vector3D(const float xpos, const float ypos, float zpos) : m_x(xpos), m_y(ypos), m_z(zpos) {}
  explicit Vector3D(const Point& point)
  : m_x(static_cast<float>(point.x())), m_y(static_cast<float>(point.y())), m_z(0.f) {}
  explicit Vector3D(const PointF& point) : m_x(point.x()), m_y(point.y()), m_z(0.f) {}
  explicit Vector3D(const Vector2D& vector);
  Vector3D(const Vector2D& vector, float zpos);
  explicit Vector3D(const Vector4D& vector);

  [[nodiscard]] bool isNull() const { return ::isNull(m_x) && ::isNull(m_y) && ::isNull(m_z); }

  [[nodiscard]] float x() const { return m_x; }
  [[nodiscard]] float y() const { return m_y; }
  [[nodiscard]] float z() const { return m_z; }
  void setX(const float x) { m_x = x; }
  void setY(const float y) { m_y = y; }
  void setZ(const float z) { m_z = z; }
  [[nodiscard]] float length() const;
  [[nodiscard]] float lengthSquared() const;

  [[nodiscard]] Vector3D normalized() const;
  void normalize();

  Vector3D& operator+=(const Vector3D& vector) {
    m_x += vector.m_x;
    m_y += vector.m_y;
    m_z += vector.m_z;
    return *this;
  }
  Vector3D& operator-=(const Vector3D& vector) {
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    m_z -= vector.m_z;
    return *this;
  }
  Vector3D& operator*=(const float factor) {
    m_x *= factor;
    m_y *= factor;
    m_z *= factor;
    return *this;
  }
  Vector3D& operator*=(const Vector3D& vector) {
    m_x *= vector.m_x;
    m_y *= vector.m_y;
    m_z *= vector.m_z;
    return *this;
  }
  Vector3D& operator/=(const float divisor) {
    m_x /= divisor;
    m_y /= divisor;
    m_z /= divisor;
    return *this;
  }

  static float dotProduct(const Vector3D& v1, const Vector3D& v2);
  static Vector3D crossProduct(const Vector3D& v1, const Vector3D& v2);
  static Vector3D normal(const Vector3D& v1, const Vector3D& v2);
  static Vector3D normal(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3);

  [[nodiscard]] float distanceToPlane(const Vector3D& plane, const Vector3D& normal) const;
  [[nodiscard]] float distanceToPlane(const Vector3D& plane1, const Vector3D& plane2, const Vector3D& plane3) const;
  [[nodiscard]] float distanceToLine(const Vector3D& point, const Vector3D& direction) const;

  friend inline bool operator==(const Vector3D& v1, const Vector3D& v2);
  friend inline bool operator!=(const Vector3D& v1, const Vector3D& v2);
  friend inline Vector3D operator+(const Vector3D& v1, const Vector3D& v2);
  friend inline Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
  friend inline Vector3D operator*(float factor, const Vector3D& vector);
  friend inline Vector3D operator*(const Vector3D& vector, float factor);
  friend inline Vector3D operator*(const Vector3D& v1, const Vector3D& v2);
  friend inline Vector3D operator-(const Vector3D& vector);
  friend inline Vector3D operator/(const Vector3D& vector, float divisor);

  friend inline bool fuzzyCompare(const Vector3D& v1, const Vector3D& v2);

  [[nodiscard]] Vector2D toVector2D() const;
  [[nodiscard]] Vector4D toVector4D() const;

  [[nodiscard]] Point toPoint() const;
  [[nodiscard]] PointF toPointF() const;

private:
  float m_x;
  float m_y;
  float m_z;

  friend class Vector2D;
  friend class Vector4D;

  friend inline Vector3D operator*(const Vector3D& vector, const Matrix4x4& matrix);
  friend inline Vector3D operator*(const Matrix4x4& matrix, const Vector3D& vector);
};

inline bool operator==(const Vector3D& v1, const Vector3D& v2) {
  return v1.m_x == v2.m_x && v1.m_y == v2.m_y && v1.m_z == v2.m_z;
}

inline bool operator!=(const Vector3D& v1, const Vector3D& v2) {
  return v1.m_x != v2.m_x || v1.m_y != v2.m_y || v1.m_z != v2.m_z;
}

inline Vector3D operator+(const Vector3D& v1, const Vector3D& v2) {
  return {v1.m_x + v2.m_x, v1.m_y + v2.m_y, v1.m_z + v2.m_z};
}

inline Vector3D operator-(const Vector3D& v1, const Vector3D& v2) {
  return {v1.m_x - v2.m_x, v1.m_y - v2.m_y, v1.m_z - v2.m_z};
}

inline Vector3D operator*(const Vector3D& vector, const float factor) {
  return {factor * vector.m_x, factor * vector.m_y, factor * vector.m_z};
}
inline Vector3D operator*(const float factor, const Vector3D& vector) {
  return {factor * vector.m_x, factor * vector.m_y, factor * vector.m_z};
}
inline Vector3D operator*(const Vector3D& v1, const Vector3D& v2) {
  return {v1.m_x * v2.m_x, v1.m_y * v2.m_y, v1.m_z * v2.m_z};
}

inline Vector3D operator-(const Vector3D& vector) { return {-vector.m_x, -vector.m_y, -vector.m_z}; }
inline Vector3D operator/(const Vector3D& vector, const float divisor) {
  return {vector.m_x / divisor, vector.m_y / divisor, vector.m_z / divisor};
}

inline bool fuzzyCompare(const Vector3D& v1, const Vector3D& v2) {
  return fuzzyCompare(v1.m_x, v2.m_x) && fuzzyCompare(v1.m_y, v2.m_y) && fuzzyCompare(v1.m_z, v2.m_z);
}