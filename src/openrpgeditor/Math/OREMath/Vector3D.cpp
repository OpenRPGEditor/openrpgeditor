#include "Vector3D.hpp"

#include "Vector2D.hpp"
#include "Vector4D.hpp"

Vector3D::Vector3D(const Vector2D& vector) : m_x(vector.m_x), m_y(vector.m_y), m_z(0.f) {}
Vector3D::Vector3D(const Vector2D& vector, const float zpos) : m_x(vector.m_x), m_y(vector.m_y), m_z(zpos) {}

Vector3D::Vector3D(const Vector4D& vector) : m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z) {}

Vector3D Vector3D::normalized() const {
  double len = static_cast<double>(m_x) * static_cast<double>(m_x) + static_cast<double>(m_y) * static_cast<double>(m_y) + static_cast<double>(m_z) * static_cast<double>(m_z);
  if (fuzzyIsNull(len - 1.0f))
    return *this;
  if (!fuzzyIsNull(len)) {
    return *this / std::sqrt(len);
  }

  return {};
}

void Vector3D::normalize() {
  double len = static_cast<double>(m_x) * static_cast<double>(m_x) + static_cast<double>(m_y) * static_cast<double>(m_y) + static_cast<double>(m_z) * static_cast<double>(m_z);
  if (fuzzyIsNull(len - 1.0f) || fuzzyIsNull(len)) {
    return;
  }

  len = std::sqrt(len);

  m_x /= len;
  m_y /= len;
  m_z /= len;
}

float Vector3D::dotProduct(const Vector3D& v1, const Vector3D& v2) { return v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z; }

Vector3D Vector3D::crossProduct(const Vector3D& v1, const Vector3D& v2) { return {v1.m_y * v2.m_z - v1.m_z * v2.m_y, v1.m_z * v2.m_x - v1.m_x * v2.m_z, v1.m_x * v2.m_y - v1.m_y * v2.m_x}; }

Vector3D Vector3D::normal(const Vector3D& v1, const Vector3D& v2) { return crossProduct(v1, v2).normalized(); }

Vector3D Vector3D::normal(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3) { return crossProduct((v2 - v1), (v3 - v1)).normalized(); }

float Vector3D::distanceToPlane(const Vector3D& plane, const Vector3D& normal) const { return dotProduct(*this - plane, normal); }

float Vector3D::distanceToPlane(const Vector3D& plane1, const Vector3D& plane2, const Vector3D& plane3) const {
  const Vector3D n = normal(plane2 - plane1, plane3 - plane1);
  return dotProduct(*this - plane1, n);
}
float Vector3D::distanceToLine(const Vector3D& point, const Vector3D& direction) const {
  if (direction.isNull())
    return (*this - point).length();
  const Vector3D p = point + dotProduct(*this - point, direction) * direction;
  return (*this - p).length();
}

Vector2D Vector3D::toVector2D() const { return {m_x, m_y}; }
Vector4D Vector3D::toVector4D() const { return {m_x, m_y, m_z, 0.0f}; }