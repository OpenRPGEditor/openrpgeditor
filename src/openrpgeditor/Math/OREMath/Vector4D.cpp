#include "Vector4D.hpp"

#include "Vector2D.hpp"
#include "Vector3D.hpp"

#include <cmath>

Vector4D::Vector4D(const Vector2D& vector)
: m_x(vector.m_x)
, m_y(vector.m_y)
, m_z(0.f)
, m_w(0.f) {}
Vector4D::Vector4D(const Vector2D& vector, const float zpos, const float wpos)
: m_x(vector.m_x)
, m_y(vector.m_y)
, m_z(zpos)
, m_w(wpos) {}

Vector4D::Vector4D(const Vector3D& vector)
: m_x(vector.m_x)
, m_y(vector.m_y)
, m_z(0.f)
, m_w(0.f) {}
Vector4D::Vector4D(const Vector3D& vector, const float wpos)
: m_x(vector.m_x)
, m_y(vector.m_y)
, m_z(vector.m_z)
, m_w(wpos) {}

float Vector4D::length() const { return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z); }

float Vector4D::lengthSquared() const { return m_x * m_x + m_y * m_y + m_z * m_z; }

Vector4D Vector4D::normalized() const {
  const double len = static_cast<double>(m_x) * static_cast<double>(m_x) + static_cast<double>(m_y) * static_cast<double>(m_y) + static_cast<double>(m_z) * static_cast<double>(m_z) +
                     static_cast<double>(m_w) * static_cast<double>(m_w);
  if (fuzzyIsNull(len - 1.0f))
    return *this;
  if (!fuzzyIsNull(len))
    return *this / sqrtf(len);
  return {};
}

void Vector4D::normalize() {
  double len = static_cast<double>(m_x) * static_cast<double>(m_x) + static_cast<double>(m_y) * static_cast<double>(m_y) + static_cast<double>(m_z) * static_cast<double>(m_z) +
               static_cast<double>(m_w) * static_cast<double>(m_w);
  if (fuzzyIsNull(len - 1.0f) || fuzzyIsNull(len)) {
    return;
  }

  len = sqrtf(len);

  m_x /= len;
  m_y /= len;
  m_z /= len;
  m_w /= len;
}

float Vector4D::dotProduct(const Vector4D& v1, const Vector4D& v2) { return v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z + v1.m_w * v2.m_w; }

Vector2D Vector4D::toVector2D() const { return {m_x, m_y}; }

Vector2D Vector4D::toVector2DAffine() const {
  if (::isNull(m_w)) {
    return {};
  }
  return {m_x / m_w, m_y / m_w};
}

Vector3D Vector4D::toVector3D() const { return {m_x, m_y, m_z}; }

Vector3D Vector4D::toVector3DAffine() const {
  if (::isNull(m_w)) {
    return {};
  }
  return {m_x / m_w, m_y / m_w, m_z / m_w};
}