
#include "Vector2D.hpp"

#include "Vector3D.hpp"
#include "Vector4D.hpp"

Vector2D::Vector2D(const Vector3D& vector) : m_x(vector.m_x), m_y(vector.m_y) {}
Vector2D::Vector2D(const Vector4D& vector) : m_x(vector.m_x), m_y(vector.m_y) {}

float Vector2D::length() const { return sqrtf(m_x * m_x + m_y * m_y); }

float Vector2D::lengthSquared() const { return m_x * m_x + m_y * m_y; }

Vector2D Vector2D::normalized() const {
  const double len = static_cast<double>(m_x) * static_cast<double>(m_x) + static_cast<double>(m_y) * static_cast<double>(m_y);
  if (fuzzyIsNull(len - 1.0f)) {
    return *this;
  }
  if (!fuzzyIsNull(len)) {
    return *this / std::sqrt(len);
  }
  return {};
}

void Vector2D::normalize() {
  double len = static_cast<double>(m_x) * static_cast<double>(m_x) + static_cast<double>(m_y) * static_cast<double>(m_y);
  if (fuzzyIsNull(len - 1.0f) || fuzzyIsNull(len)) {
    return;
  }

  len = std::sqrt(len);

  m_x /= len;
  m_y /= len;
}

float Vector2D::dotProduct(const Vector2D& v1, const Vector2D& v2) { return v1.m_x * v2.m_x + v1.m_y * v2.m_y; }

Vector3D Vector2D::toVector3D() const { return {m_x, m_y, 0.0f}; }
