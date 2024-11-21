#pragma once
#include "MathGlobals.hpp"
#include "Point.hpp"

class Matrix4x4;

class Vector2D;
class Vector3D;

class Vector4D {
public:
  Vector4D() : m_x(0.f), m_y(0.f), m_z(0.f), m_w(0.f) {}
  Vector4D(float xpos, float ypos, float zpos, float wpos) : m_x(xpos), m_y(ypos), m_z(zpos), m_w(wpos) {}
  explicit Vector4D(const Point& point)
  : m_x(static_cast<float>(point.x())), m_y(static_cast<float>(point.y())), m_z(0.f), m_w(0.f) {}
  explicit Vector4D(const PointF& point) : m_x(point.x()), m_y(point.y()), m_z(0.f), m_w(0.f) {}

  explicit Vector4D(const Vector2D& vector);
  Vector4D(const Vector2D& vector, float m_zos, float m_wos);

  explicit Vector4D(const Vector3D& vector);
  Vector4D(const Vector3D& vector, float m_wos);

  [[nodiscard]] bool isNull() const { return ::isNull(m_x) && ::isNull(m_y) && ::isNull(m_z) && ::isNull(m_w); }

  [[nodiscard]] float x() const { return m_x; }
  [[nodiscard]] float y() const { return m_y; }
  [[nodiscard]] float z() const { return m_z; }
  [[nodiscard]] float w() const { return m_w; }

  void setX(const float aX) { m_x = aX; }
  void setY(const float aY) { m_y = aY; }
  void setZ(const float aZ) { m_z = aZ; }
  void setW(const float aW) { m_w = aW; }

  [[nodiscard]] float length() const;
  [[nodiscard]] float lengthSquared() const;

  [[nodiscard]] Vector4D normalized() const;
  void normalize();

  Vector4D& operator+=(const Vector4D& vector) {
    m_x += vector.m_x;
    m_y += vector.m_y;
    m_z += vector.m_z;
    m_w += vector.m_w;
    return *this;
  }
  Vector4D& operator-=(const Vector4D& vector) {
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    m_z -= vector.m_z;
    m_w -= vector.m_w;
    return *this;
  }
  Vector4D& operator*=(const float factor) {
    m_x *= factor;
    m_y *= factor;
    m_z *= factor;
    m_w *= factor;
    return *this;
  }
  Vector4D& operator*=(const Vector4D& vector) {
    m_x *= vector.m_x;
    m_y *= vector.m_y;
    m_z *= vector.m_z;
    m_w *= vector.m_w;
    return *this;
  }
  Vector4D& operator/=(float divisor) {
    m_x /= divisor;
    m_y /= divisor;
    m_z /= divisor;
    m_w /= divisor;
    return *this;
  }

  static float dotProduct(const Vector4D& v1, const Vector4D& v2);

  friend inline bool operator==(const Vector4D& v1, const Vector4D& v2);
  friend inline bool operator!=(const Vector4D& v1, const Vector4D& v2);
  friend inline Vector4D operator+(const Vector4D& v1, const Vector4D& v2);
  friend inline Vector4D operator-(const Vector4D& v1, const Vector4D& v2);
  friend inline Vector4D operator*(float factor, const Vector4D& vector);
  friend inline Vector4D operator*(const Vector4D& vector, float factor);
  friend inline Vector4D operator*(const Vector4D& v1, const Vector4D& v2);
  friend inline Vector4D operator-(const Vector4D& vector);
  friend inline Vector4D operator/(const Vector4D& vector, float divisor);

  friend inline bool fuzzyCompare(const Vector4D& v1, const Vector4D& v2);

  [[nodiscard]] Vector2D toVector2D() const;
  [[nodiscard]] Vector2D toVector2DAffine() const;

  [[nodiscard]] Vector3D toVector3D() const;
  [[nodiscard]] Vector3D toVector3DAffine() const;

  [[nodiscard]] Point toPoint() const { return {oRound(m_x), oRound(m_y)}; }

  [[nodiscard]] PointF toPointF() const { return {m_x, m_y}; }

#ifdef IMGUI_VERSION
  explicit operator ImVec4() const { return {m_x, m_y, m_z, m_w}; }
#endif

private:
  float m_x;
  float m_y;
  float m_z;
  float m_w;

  friend class Vector2D;
  friend class Vector3D;
  friend Vector4D operator*(const Vector4D& vector, const Matrix4x4& matrix);
  friend Vector4D operator*(const Matrix4x4& matrix, const Vector4D& vector);
};

inline bool operator==(const Vector4D& v1, const Vector4D& v2) {
  return v1.m_x == v2.m_x && v1.m_y == v2.m_y && v1.m_z == v2.m_z && v1.m_w == v2.m_w;
}

inline bool operator!=(const Vector4D& v1, const Vector4D& v2) {
  return v1.m_x != v2.m_x || v1.m_y != v2.m_y || v1.m_z != v2.m_z || v1.m_w != v2.m_w;
}

inline Vector4D operator+(const Vector4D& v1, const Vector4D& v2) {
  return {v1.m_x + v2.m_x, v1.m_y + v2.m_y, v1.m_z + v2.m_z, v1.m_w + v2.m_w};
}

inline Vector4D operator-(const Vector4D& v1, const Vector4D& v2) {
  return {v1.m_x - v2.m_x, v1.m_y - v2.m_y, v1.m_z - v2.m_z, v1.m_w - v2.m_w};
}

inline Vector4D operator*(float factor, const Vector4D& vector) {
  return {vector.m_x * factor, vector.m_y * factor, vector.m_z * factor, vector.m_w * factor};
}

inline Vector4D operator*(const Vector4D& vector, const float factor) {
  return {vector.m_x * factor, vector.m_y * factor, vector.m_z * factor, vector.m_w * factor};
}

inline Vector4D operator*(const Vector4D& v1, const Vector4D& v2) {
  return {v1.m_x * v2.m_x, v1.m_y * v2.m_y, v1.m_z * v2.m_z, v1.m_w * v2.m_w};
}

inline Vector4D operator-(const Vector4D& vector) { return {-vector.m_x, -vector.m_y, -vector.m_z, -vector.m_w}; }

inline Vector4D operator/(const Vector4D& vector, float divisor) {
  return {vector.m_x / divisor, vector.m_y / divisor, vector.m_z / divisor, vector.m_w / divisor};
}

inline bool fuzzyCompare(const Vector4D& v1, const Vector4D& v2) {
  return fuzzyCompare(v1.m_x, v2.m_x) && fuzzyCompare(v1.m_y, v2.m_y) && fuzzyCompare(v1.m_z, v2.m_z) &&
         fuzzyCompare(v1.m_w, v2.m_w);
}
