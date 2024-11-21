// ReSharper disable CppTooWideScopeInitStatement
#include "Matrix4x4.hpp"
#include <cmath>

static constexpr float InvDistToPlane = 1.f / 1024.f;

Matrix4x4::Matrix4x4(const float* values) {
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      m[col][row] = values[row * 4 + col];
    }
  }
  flagBits = General;
}

Matrix4x4::Matrix4x4(const float* values, const int cols, const int rows) {
  for (int col = 0; col < 4; ++col) {
    for (int row = 0; row < 4; ++row) {
      if (col < cols && row < rows) {
        m[col][row] = values[col * rows + row];
      } else if (col == row) {
        m[col][row] = 1.0f;
      } else {
        m[col][row] = 0.0f;
      }
    }
  }
  flagBits = General;
}

static float matrixDet3(const float m[4][4], const int col0, const int col1, const int col2, const int row0,
                        const int row1, const int row2) {
  return m[col0][row0] * (m[col1][row1] * m[col2][row2] - m[col1][row2] * m[col2][row1]) -
         m[col1][row0] * (m[col0][row1] * m[col2][row2] - m[col0][row2] * m[col2][row1]) +
         m[col2][row0] * (m[col0][row1] * m[col1][row2] - m[col0][row2] * m[col1][row1]);
}

static float matrixDet4(const float m[4][4]) {
  float det = m[0][0] * matrixDet3(m, 1, 2, 3, 1, 2, 3);
  det -= m[1][0] * matrixDet3(m, 0, 2, 3, 1, 2, 3);
  det += m[2][0] * matrixDet3(m, 0, 1, 3, 1, 2, 3);
  det -= m[3][0] * matrixDet3(m, 0, 1, 2, 1, 2, 3);
  return det;
}

float Matrix4x4::determinant() const { return matrixDet4(m); }

Matrix4x4 Matrix4x4::inverted(bool* invertible) const {
  if (flagBits == Identity) {
    if (invertible) {
      *invertible = true;
    }
    return {};
  }
  if (flagBits == Translation) {
    Matrix4x4 inv;
    inv.m[3][0] = -m[3][0];
    inv.m[3][1] = -m[3][1];
    inv.m[3][2] = -m[3][2];
    inv.flagBits = Translation;
    if (invertible) {
      *invertible = true;
    }
    return inv;
  }
  if (flagBits == Rotation || flagBits == (Rotation | Translation)) {
    if (invertible) {
      *invertible = true;
    }

    return orthonormalInverse();
  }

  Matrix4x4 inv(1); // The "1" says to not load the identity.

  float det = matrixDet4(m);
  if (det == 0.0f) {
    if (invertible)
      *invertible = false;
    return {};
  }
  det = 1.0f / det;

  inv.m[0][0] = matrixDet3(m, 1, 2, 3, 1, 2, 3) * det;
  inv.m[0][1] = -matrixDet3(m, 0, 2, 3, 1, 2, 3) * det;
  inv.m[0][2] = matrixDet3(m, 0, 1, 3, 1, 2, 3) * det;
  inv.m[0][3] = -matrixDet3(m, 0, 1, 2, 1, 2, 3) * det;
  inv.m[1][0] = -matrixDet3(m, 1, 2, 3, 0, 2, 3) * det;
  inv.m[1][1] = matrixDet3(m, 0, 2, 3, 0, 2, 3) * det;
  inv.m[1][2] = -matrixDet3(m, 0, 1, 3, 0, 2, 3) * det;
  inv.m[1][3] = matrixDet3(m, 0, 1, 2, 0, 2, 3) * det;
  inv.m[2][0] = matrixDet3(m, 1, 2, 3, 0, 1, 3) * det;
  inv.m[2][1] = -matrixDet3(m, 0, 2, 3, 0, 1, 3) * det;
  inv.m[2][2] = matrixDet3(m, 0, 1, 3, 0, 1, 3) * det;
  inv.m[2][3] = -matrixDet3(m, 0, 1, 2, 0, 1, 3) * det;
  inv.m[3][0] = -matrixDet3(m, 1, 2, 3, 0, 1, 2) * det;
  inv.m[3][1] = matrixDet3(m, 0, 2, 3, 0, 1, 2) * det;
  inv.m[3][2] = -matrixDet3(m, 0, 1, 3, 0, 1, 2) * det;
  inv.m[3][3] = matrixDet3(m, 0, 1, 2, 0, 1, 2) * det;

  if (invertible)
    *invertible = true;
  return inv;
}

Matrix4x4 Matrix4x4::transposed() const {
  Matrix4x4 result(1);
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      result.m[col][row] = m[row][col];
    }
  }
  return result;
}

Matrix3x3 Matrix4x4::normalMatrix() const {
  Matrix3x3 inv;

  if (flagBits == Identity || flagBits == Translation) {
    return inv;
  }
  if (flagBits == Scale || flagBits == (Translation | Scale)) {
    if (m[0][0] == 0.0f || m[1][1] == 0.0f || m[2][2] == 0.0f)
      return inv;
    inv.data()[0] = 1.0f / m[0][0];
    inv.data()[4] = 1.0f / m[1][1];
    inv.data()[8] = 1.0f / m[2][2];
    return inv;
  }

  float det = matrixDet3(m, 0, 1, 2, 0, 1, 2);
  if (det == 0.0f) {
    return inv;
  }
  det = 1.0f / det;

  float* invm = inv.data();

  // Invert and transpose in a single step.
  invm[0 + 0 * 3] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * det;
  invm[1 + 0 * 3] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) * det;
  invm[2 + 0 * 3] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * det;
  invm[0 + 1 * 3] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * det;
  invm[1 + 1 * 3] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * det;
  invm[2 + 1 * 3] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * det;
  invm[0 + 2 * 3] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * det;
  invm[1 + 2 * 3] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * det;
  invm[2 + 2 * 3] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * det;

  return inv;
}

Matrix4x4& Matrix4x4::operator/=(const float divisor) {
  m[0][0] /= divisor;
  m[0][1] /= divisor;
  m[0][2] /= divisor;
  m[0][3] /= divisor;
  m[1][0] /= divisor;
  m[1][1] /= divisor;
  m[1][2] /= divisor;
  m[1][3] /= divisor;
  m[2][0] /= divisor;
  m[2][1] /= divisor;
  m[2][2] /= divisor;
  m[2][3] /= divisor;
  m[3][0] /= divisor;
  m[3][1] /= divisor;
  m[3][2] /= divisor;
  m[3][3] /= divisor;
  flagBits = General;
  return *this;
}

Matrix4x4 operator/(const Matrix4x4& matrix, const float divisor) {
  Matrix4x4 m(1);
  m.m[0][0] = matrix.m[0][0] / divisor;
  m.m[0][1] = matrix.m[0][1] / divisor;
  m.m[0][2] = matrix.m[0][2] / divisor;
  m.m[0][3] = matrix.m[0][3] / divisor;
  m.m[1][0] = matrix.m[1][0] / divisor;
  m.m[1][1] = matrix.m[1][1] / divisor;
  m.m[1][2] = matrix.m[1][2] / divisor;
  m.m[1][3] = matrix.m[1][3] / divisor;
  m.m[2][0] = matrix.m[2][0] / divisor;
  m.m[2][1] = matrix.m[2][1] / divisor;
  m.m[2][2] = matrix.m[2][2] / divisor;
  m.m[2][3] = matrix.m[2][3] / divisor;
  m.m[3][0] = matrix.m[3][0] / divisor;
  m.m[3][1] = matrix.m[3][1] / divisor;
  m.m[3][2] = matrix.m[3][2] / divisor;
  m.m[3][3] = matrix.m[3][3] / divisor;
  return m;
}

void Matrix4x4::scale(const Vector3D& vector) {
  const auto vx = vector.x();
  const auto vy = vector.y();
  const auto vz = vector.z();
  if (flagBits == Identity) {
    m[0][0] = vx;
    m[1][1] = vy;
    m[2][2] = vz;
    flagBits = Scale;
  } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
    m[0][0] *= vx;
    m[1][1] *= vy;
    m[2][2] *= vz;
  } else if (flagBits == Translation) {
    m[0][0] = vx;
    m[1][1] = vy;
    m[2][2] = vz;
    flagBits |= Scale;
  } else {
    m[0][0] *= vx;
    m[0][1] *= vx;
    m[0][2] *= vx;
    m[0][3] *= vx;
    m[1][0] *= vy;
    m[1][1] *= vy;
    m[1][2] *= vy;
    m[1][3] *= vy;
    m[2][0] *= vz;
    m[2][1] *= vz;
    m[2][2] *= vz;
    m[2][3] *= vz;
    flagBits = General;
  }
}
void Matrix4x4::scale(const float x, const float y) {
  if (flagBits == Identity) {
    m[0][0] = x;
    m[1][1] = y;
    flagBits = Scale;
  } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
    m[0][0] *= x;
    m[1][1] *= y;
  } else if (flagBits == Translation) {
    m[0][0] = x;
    m[1][1] = y;
    flagBits |= Scale;
  } else {
    m[0][0] *= x;
    m[0][1] *= x;
    m[0][2] *= x;
    m[0][3] *= x;
    m[1][0] *= y;
    m[1][1] *= y;
    m[1][2] *= y;
    m[1][3] *= y;
    flagBits = General;
  }
}

void Matrix4x4::scale(const float x, const float y, const float z) {
  if (flagBits == Identity) {
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
    flagBits = Scale;
  } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
    m[0][0] *= x;
    m[1][1] *= y;
    m[2][2] *= z;
  } else if (flagBits == Translation) {
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
    flagBits |= Scale;
  } else {
    m[0][0] *= x;
    m[0][1] *= x;
    m[0][2] *= x;
    m[0][3] *= x;
    m[1][0] *= y;
    m[1][1] *= y;
    m[1][2] *= y;
    m[1][3] *= y;
    m[2][0] *= z;
    m[2][1] *= z;
    m[2][2] *= z;
    m[2][3] *= z;
    flagBits = General;
  }
}

void Matrix4x4::scale(const float factor) {
  if (flagBits == Identity) {
    m[0][0] = factor;
    m[1][1] = factor;
    m[2][2] = factor;
    flagBits = Scale;
  } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
    m[0][0] *= factor;
    m[1][1] *= factor;
    m[2][2] *= factor;
  } else if (flagBits == Translation) {
    m[0][0] = factor;
    m[1][1] = factor;
    m[2][2] = factor;
    flagBits |= Scale;
  } else {
    m[0][0] *= factor;
    m[0][1] *= factor;
    m[0][2] *= factor;
    m[0][3] *= factor;
    m[1][0] *= factor;
    m[1][1] *= factor;
    m[1][2] *= factor;
    m[1][3] *= factor;
    m[2][0] *= factor;
    m[2][1] *= factor;
    m[2][2] *= factor;
    m[2][3] *= factor;
    flagBits = General;
  }
}

void Matrix4x4::translate(const Vector3D& vector) {
  const auto vx = vector.x();
  const auto vy = vector.y();
  const auto vz = vector.z();
  if (flagBits == Identity) {
    m[3][0] = vx;
    m[3][1] = vy;
    m[3][2] = vz;
    flagBits = Translation;
  } else if (flagBits == Translation) {
    m[3][0] += vx;
    m[3][1] += vy;
    m[3][2] += vz;
  } else if (flagBits == Scale) {
    m[3][0] = m[0][0] * vx;
    m[3][1] = m[1][1] * vy;
    m[3][2] = m[2][2] * vz;
    flagBits |= Translation;
  } else if (flagBits == (Scale | Translation)) {
    m[3][0] += m[0][0] * vx;
    m[3][1] += m[1][1] * vy;
    m[3][2] += m[2][2] * vz;
  } else {
    m[3][0] += m[0][0] * vx + m[1][0] * vy + m[2][0] * vz;
    m[3][1] += m[0][1] * vx + m[1][1] * vy + m[2][1] * vz;
    m[3][2] += m[0][2] * vx + m[1][2] * vy + m[2][2] * vz;
    m[3][3] += m[0][3] * vx + m[1][3] * vy + m[2][3] * vz;
    if (flagBits == Rotation) {
      flagBits |= Translation;
    } else if (flagBits != (Rotation | Translation)) {
      flagBits = General;
    }
  }
}

void Matrix4x4::translate(const float x, const float y) {
  if (flagBits == Identity) {
    m[3][0] = x;
    m[3][1] = y;
    flagBits = Translation;
  } else if (flagBits == Translation) {
    m[3][0] += x;
    m[3][1] += y;
  } else if (flagBits == Scale) {
    m[3][0] = m[0][0] * x;
    m[3][1] = m[1][1] * y;
    m[3][2] = 0.;
    flagBits |= Translation;
  } else if (flagBits == (Scale | Translation)) {
    m[3][0] += m[0][0] * x;
    m[3][1] += m[1][1] * y;
  } else {
    m[3][0] += m[0][0] * x + m[1][0] * y;
    m[3][1] += m[0][1] * x + m[1][1] * y;
    m[3][2] += m[0][2] * x + m[1][2] * y;
    m[3][3] += m[0][3] * x + m[1][3] * y;
    if (flagBits == Rotation) {
      flagBits |= Translation;
    } else if (flagBits != (Rotation | Translation)) {
      flagBits = General;
    }
  }
}

void Matrix4x4::translate(const float x, const float y, const float z) {
  if (flagBits == Identity) {
    m[3][0] = x;
    m[3][1] = y;
    m[3][2] = z;
    flagBits = Translation;
  } else if (flagBits == Translation) {
    m[3][0] += x;
    m[3][1] += y;
    m[3][2] += z;
  } else if (flagBits == Scale) {
    m[3][0] = m[0][0] * x;
    m[3][1] = m[1][1] * y;
    m[3][2] = m[2][2] * z;
    flagBits |= Translation;
  } else if (flagBits == (Scale | Translation)) {
    m[3][0] += m[0][0] * x;
    m[3][1] += m[1][1] * y;
    m[3][2] += m[2][2] * z;
  } else {
    m[3][0] += m[0][0] * x + m[1][0] * y + m[2][0] * z;
    m[3][1] += m[0][1] * x + m[1][1] * y + m[2][1] * z;
    m[3][2] += m[0][2] * x + m[1][2] * y + m[2][2] * z;
    m[3][3] += m[0][3] * x + m[1][3] * y + m[2][3] * z;
    if (flagBits == Rotation) {
      flagBits |= Translation;
    } else if (flagBits != (Rotation | Translation)) {
      flagBits = General;
    }
  }
}

void Matrix4x4::rotate(const float angle, const Vector3D& vector) { rotate(angle, vector.x(), vector.y(), vector.z()); }

void Matrix4x4::rotate(const float angle, float x, float y, float z) {
  if (angle == 0.0f) {
    return;
  }
  Matrix4x4 m(1);
  float c, s;
  if (angle == 90.0f || angle == -270.0f) {
    s = 1.0f;
    c = 0.0f;
  } else if (angle == -90.0f || angle == 270.0f) {
    s = -1.0f;
    c = 0.0f;
  } else if (angle == 180.0f || angle == -180.0f) {
    s = 0.0f;
    c = -1.0f;
  } else {
    const float a = angle * static_cast<float>(M_PI) / 180.0f;
    c = std::cos(a);
    s = std::sin(a);
  }
  bool quick = false;
  if (x == 0.0f) {
    if (y == 0.0f) {
      if (z != 0.0f) {
        m.setToIdentity();
        m.m[0][0] = c;
        m.m[1][1] = c;
        if (z < 0.0f) {
          m.m[1][0] = s;
          m.m[0][1] = -s;
        } else {
          m.m[1][0] = -s;
          m.m[0][1] = s;
        }
        m.flagBits = General;
        quick = true;
      }
    } else if (z == 0.0f) {
      m.setToIdentity();
      m.m[0][0] = c;
      m.m[2][2] = c;
      if (y < 0.0f) {
        m.m[2][0] = -s;
        m.m[0][2] = s;
      } else {
        m.m[2][0] = s;
        m.m[0][2] = -s;
      }
      m.flagBits = General;
      quick = true;
    }
  } else if (y == 0.0f && z == 0.0f) {
    m.setToIdentity();
    m.m[1][1] = c;
    m.m[2][2] = c;
    if (x < 0.0f) {
      m.m[2][1] = s;
      m.m[1][2] = -s;
    } else {
      m.m[2][1] = -s;
      m.m[1][2] = s;
    }
    m.flagBits = General;
    quick = true;
  }
  if (!quick) {
    if (auto len = x * x + y * y + z * z; !fuzzyIsNull(len - 1.0f) && !fuzzyIsNull(len)) {
      len = std::sqrtf(len);
      x /= len;
      y /= len;
      z /= len;
    }
    const float ic = 1.0f - c;
    m.m[0][0] = x * x * ic + c;
    m.m[1][0] = x * y * ic - z * s;
    m.m[2][0] = x * z * ic + y * s;
    m.m[3][0] = 0.0f;
    m.m[0][1] = y * x * ic + z * s;
    m.m[1][1] = y * y * ic + c;
    m.m[2][1] = y * z * ic - x * s;
    m.m[3][1] = 0.0f;
    m.m[0][2] = x * z * ic - y * s;
    m.m[1][2] = y * z * ic + x * s;
    m.m[2][2] = z * z * ic + c;
    m.m[3][2] = 0.0f;
    m.m[0][3] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][3] = 0.0f;
    m.m[3][3] = 1.0f;
  }
  const int flags = flagBits;
  *this *= m;
  if (flags != Identity) {
    flagBits = flags | Rotation;
  } else {
    flagBits = Rotation;
  }
}

void Matrix4x4::projectedRotate(const float angle, float x, float y, float z) {
  if (angle == 0.0f) {
    return;
  }
  Matrix4x4 m(1);
  float c, s;
  if (angle == 90.0f || angle == -270.0f) {
    s = 1.0f;
    c = 0.0f;
  } else if (angle == -90.0f || angle == 270.0f) {
    s = -1.0f;
    c = 0.0f;
  } else if (angle == 180.0f || angle == -180.0f) {
    s = 0.0f;
    c = -1.0f;
  } else {
    const float a = angle * static_cast<float>(M_PI) / 180.0f;
    c = std::cos(a);
    s = std::sin(a);
  }
  bool quick = false;
  if (x == 0.0f) {
    if (y == 0.0f) {
      if (z != 0.0f) {
        m.setToIdentity();
        m.m[0][0] = c;
        m.m[1][1] = c;
        if (z < 0.0f) {
          m.m[1][0] = s;
          m.m[0][1] = -s;
        } else {
          m.m[1][0] = -s;
          m.m[0][1] = s;
        }
        m.flagBits = General;
        quick = true;
      }
    } else if (z == 0.0f) {
      m.setToIdentity();
      m.m[0][0] = c;
      m.m[2][2] = 1.0f;
      if (y < 0.0f) {
        m.m[0][3] = -s * InvDistToPlane;
      } else {
        m.m[0][3] = s * InvDistToPlane;
      }
      m.flagBits = General;
      quick = true;
    }
  } else if (y == 0.0f && z == 0.0f) {
    m.setToIdentity();
    m.m[1][1] = c;
    m.m[2][2] = 1.0f;
    if (x < 0.0f) {
      m.m[1][3] = s * InvDistToPlane;
    } else {
      m.m[1][3] = -s * InvDistToPlane;
    }
    m.flagBits = General;
    quick = true;
  }
  if (!quick) {
    if (float len = x * x + y * y + z * z; !fuzzyIsNull(len - 1.0f) && !fuzzyIsNull(len)) {
      len = std::sqrtf(len);
      x /= len;
      y /= len;
      z /= len;
    }
    const float ic = 1.0f - c;
    m.m[0][0] = x * x * ic + c;
    m.m[1][0] = x * y * ic - z * s;
    m.m[2][0] = 0.0f;
    m.m[3][0] = 0.0f;
    m.m[0][1] = y * x * ic + z * s;
    m.m[1][1] = y * y * ic + c;
    m.m[2][1] = 0.0f;
    m.m[3][1] = 0.0f;
    m.m[0][2] = 0.0f;
    m.m[1][2] = 0.0f;
    m.m[2][2] = 1.0f;
    m.m[3][2] = 0.0f;
    m.m[0][3] = (x * z * ic - y * s) * -InvDistToPlane;
    m.m[1][3] = (y * z * ic + x * s) * -InvDistToPlane;
    m.m[2][3] = 0.0f;
    m.m[3][3] = 1.0f;
  }
  const int flags = flagBits;
  *this *= m;
  if (flags != Identity) {
    flagBits = flags | Rotation;
  } else {
    flagBits = Rotation;
  }
}

void Matrix4x4::ortho(const Rect& rect) {
  ortho(static_cast<float>(rect.x()), static_cast<float>(rect.x() + rect.width()),
        static_cast<float>(rect.y() + rect.height()), static_cast<float>(rect.y()), -1.0f, 1.0f);
}

void Matrix4x4::ortho(const RectF& rect) { ortho(rect.left(), rect.right(), rect.bottom(), rect.top(), -1.0f, 1.0f); }

void Matrix4x4::ortho(const float left, const float right, const float bottom, const float top, const float nearPlane,
                      const float farPlane) {
  if (left == right || bottom == top || nearPlane == farPlane) {
    return;
  }

  const float width = right - left;
  const float invheight = top - bottom;
  const float clip = farPlane - nearPlane;
  if (clip == 2.0f && nearPlane + farPlane == 0.0f) {
    translate({-(left + right) / width, -(top + bottom) / invheight, 0.0f});
    scale({2.0f / width, 2.0f / invheight, -1.0f});
    return;
  }
  Matrix4x4 m(1);
  m.m[0][0] = 2.0f / width;
  m.m[1][0] = 0.0f;
  m.m[2][0] = 0.0f;
  m.m[3][0] = -(left + right) / width;
  m.m[0][1] = 0.0f;
  m.m[1][1] = 2.0f / invheight;
  m.m[2][1] = 0.0f;
  m.m[3][1] = -(top + bottom) / invheight;
  m.m[0][2] = 0.0f;
  m.m[1][2] = 0.0f;
  m.m[2][2] = -2.0f / clip;
  m.m[3][2] = -(nearPlane + farPlane) / clip;
  m.m[0][3] = 0.0f;
  m.m[1][3] = 0.0f;
  m.m[2][3] = 0.0f;
  m.m[3][3] = 1.0f;

  *this *= m;
}

void Matrix4x4::frustum(const float left, const float right, const float bottom, const float top, const float nearPlane,
                        const float farPlane) {
  if (left == right || bottom == top || nearPlane == farPlane) {
    return;
  }

  Matrix4x4 m(1);
  const float width = right - left;
  const float invheight = top - bottom;
  const float clip = farPlane - nearPlane;
  m.m[0][0] = 2.0f * nearPlane / width;
  m.m[1][0] = 0.0f;
  m.m[2][0] = (left + right) / width;
  m.m[3][0] = 0.0f;
  m.m[0][1] = 0.0f;
  m.m[1][1] = 2.0f * nearPlane / invheight;
  m.m[2][1] = (top + bottom) / invheight;
  m.m[3][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[1][2] = 0.0f;
  m.m[2][2] = -(nearPlane + farPlane) / clip;
  m.m[3][2] = -2.0f * nearPlane * farPlane / clip;
  m.m[0][3] = 0.0f;
  m.m[1][3] = 0.0f;
  m.m[2][3] = -1.0f;
  m.m[3][3] = 0.0f;
  *this *= m;
}

void Matrix4x4::perspective(const float angle, const float aspect, const float nearPlane, const float farPlane) {
  if (nearPlane == farPlane || aspect == 0.0f) {
    return;
  }
  Matrix4x4 m(1);
  const float radians = angle / 2.0f * static_cast<float>(M_PI) / 180.0f;
  const float sine = std::sin(radians);
  if (sine == 0.0f)
    return;
  const float cotan = std::cos(radians) / sine;
  const float clip = farPlane - nearPlane;
  m.m[0][0] = cotan / aspect;
  m.m[1][0] = 0.0f;
  m.m[2][0] = 0.0f;
  m.m[3][0] = 0.0f;
  m.m[0][1] = 0.0f;
  m.m[1][1] = cotan;
  m.m[2][1] = 0.0f;
  m.m[3][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[1][2] = 0.0f;
  m.m[2][2] = -(nearPlane + farPlane) / clip;
  m.m[3][2] = -(2.0f * nearPlane * farPlane) / clip;
  m.m[0][3] = 0.0f;
  m.m[1][3] = 0.0f;
  m.m[2][3] = -1.0f;
  m.m[3][3] = 0.0f;

  *this *= m;
}

void Matrix4x4::lookAt(const Vector3D& eye, const Vector3D& center, const Vector3D& up) {
  const auto forward = (center - eye).normalized();
  const auto side = Vector3D::crossProduct(forward, up).normalized();
  const auto upVector = Vector3D::crossProduct(side, forward);

  Matrix4x4 m(1);

  m.m[0][0] = side.x();
  m.m[1][0] = side.y();
  m.m[2][0] = side.z();
  m.m[3][0] = 0.0f;
  m.m[0][1] = upVector.x();
  m.m[1][1] = upVector.y();
  m.m[2][1] = upVector.z();
  m.m[3][1] = 0.0f;
  m.m[0][2] = -forward.x();
  m.m[1][2] = -forward.y();
  m.m[2][2] = -forward.z();
  m.m[3][2] = 0.0f;
  m.m[0][3] = 0.0f;
  m.m[1][3] = 0.0f;
  m.m[2][3] = 0.0f;
  m.m[3][3] = 1.0f;

  *this *= m;
  translate(-eye);
}

void Matrix4x4::flipCoordinates() {
  if (flagBits == Scale || flagBits == (Scale | Translation)) {
    m[1][1] = -m[1][1];
    m[2][2] = -m[2][2];
  } else if (flagBits == Translation) {
    m[1][1] = -m[1][1];
    m[2][2] = -m[2][2];
    flagBits |= Scale;
  } else if (flagBits == Identity) {
    m[1][1] = -1.0f;
    m[2][2] = -1.0f;
    flagBits = Scale;
  } else {
    m[1][0] = -m[1][0];
    m[1][1] = -m[1][1];
    m[1][2] = -m[1][2];
    m[1][3] = -m[1][3];
    m[2][0] = -m[2][0];
    m[2][1] = -m[2][1];
    m[2][2] = -m[2][2];
    m[2][3] = -m[2][3];
    flagBits = General;
  }
}

void Matrix4x4::copyDataTo(float* values) const {
  for (int row = 0; row < 4; ++row)
    for (int col = 0; col < 4; ++col)
      values[row * 4 + col] = m[col][row];
}

Rect Matrix4x4::mapRect(const Rect& rect) const {
  if (flagBits == (Translation | Scale) || flagBits == Scale) {
    auto x = static_cast<float>(rect.x()) * m[0][0] + m[3][0];
    auto y = static_cast<float>(rect.y()) * m[1][1] + m[3][1];
    auto w = static_cast<float>(rect.width()) * m[0][0];
    auto h = static_cast<float>(rect.height()) * m[1][1];
    if (w < 0) {
      w = -w;
      x -= w;
    }
    if (h < 0) {
      h = -h;
      y -= h;
    }
    return {oRound(x), oRound(y), oRound(w), oRound(h)};
  }
  if (flagBits == Translation) {
    return {oRound(static_cast<float>(rect.x()) + m[3][0]), oRound(static_cast<float>(rect.y()) + m[3][1]),
            rect.width(), rect.height()};
  }

  const Point tl = map(rect.topLeft());
  const Point tr = map(Point(rect.x() + rect.width(), rect.y()));
  const Point bl = map(Point(rect.x(), rect.y() + rect.height()));
  const Point br = map(Point(rect.x() + rect.width(), rect.y() + rect.height()));

  const int xmin = std::min(std::min(tl.x(), tr.x()), std::min(bl.x(), br.x()));
  const int xmax = std::max(std::max(tl.x(), tr.x()), std::max(bl.x(), br.x()));
  const int ymin = std::min(std::min(tl.y(), tr.y()), std::min(bl.y(), br.y()));
  const int ymax = std::max(std::max(tl.y(), tr.y()), std::max(bl.y(), br.y()));

  return {xmin, ymin, xmax - xmin, ymax - ymin};
}

RectF Matrix4x4::mapRect(const RectF& rect) const {
  if (flagBits == (Translation | Scale) || flagBits == Scale) {
    auto x = rect.x() * m[0][0] + m[3][0];
    auto y = rect.y() * m[1][1] + m[3][1];
    auto w = rect.width() * m[0][0];
    auto h = rect.height() * m[1][1];
    if (w < 0) {
      w = -w;
      x -= w;
    }
    if (h < 0) {
      h = -h;
      y -= h;
    }
    return {x, y, w, h};
  }
  if (flagBits == Translation) {
    return rect.translated(m[3][0], m[3][1]);
  }

  const auto tl = map(rect.topLeft());
  const auto tr = map(rect.topRight());
  const auto bl = map(rect.bottomLeft());
  const auto br = map(rect.bottomRight());

  const auto xmin = std::min(std::min(tl.x(), tr.x()), std::min(bl.x(), br.x()));
  const auto xmax = std::max(std::max(tl.x(), tr.x()), std::max(bl.x(), br.x()));
  const auto ymin = std::min(std::min(tl.y(), tr.y()), std::min(bl.y(), br.y()));
  const auto ymax = std::max(std::max(tl.y(), tr.y()), std::max(bl.y(), br.y()));

  return {PointF{xmin, ymin}, PointF{xmax, ymax}};
}

Matrix4x4 Matrix4x4::orthonormalInverse() const {
  Matrix4x4 result(1);
  result.m[0][0] = m[0][0];
  result.m[1][0] = m[0][1];
  result.m[2][0] = m[0][2];
  result.m[0][1] = m[1][0];
  result.m[1][1] = m[1][1];
  result.m[2][1] = m[1][2];
  result.m[0][2] = m[2][0];
  result.m[1][2] = m[2][1];
  result.m[2][2] = m[2][2];
  result.m[0][3] = 0.0f;
  result.m[1][3] = 0.0f;
  result.m[2][3] = 0.0f;
  result.m[3][0] = -(result.m[0][0] * m[3][0] + result.m[1][0] * m[3][1] + result.m[2][0] * m[3][2]);
  result.m[3][1] = -(result.m[0][1] * m[3][0] + result.m[1][1] * m[3][1] + result.m[2][1] * m[3][2]);
  result.m[3][2] = -(result.m[0][2] * m[3][0] + result.m[1][2] * m[3][1] + result.m[2][2] * m[3][2]);
  result.m[3][3] = 1.0f;

  return result;
}
void Matrix4x4::optimize() {
  if (m[3][3] != 1.0f) {
    flagBits = General;
    return;
  }

  if (m[1][0] != 0.0f || m[2][0] != 0.0f || m[2][1] != 0.0f) {
    flagBits = General;
    return;
  }
  if (m[0][1] != 0.0f || m[0][2] != 0.0f || m[0][3] != 0.0f || m[1][2] != 0.0f || m[1][3] != 0.0f || m[2][3] != 0.0f) {
    flagBits = General;
    return;
  }

  const bool identityAlongDiagonal = m[0][0] == 1.0f && m[1][1] == 1.0f && m[2][2] == 1.0f;
  const bool translationPresent = m[3][0] != 0.0f || m[3][1] != 0.0f || m[3][2] != 0.0f;

  if (translationPresent && identityAlongDiagonal)
    flagBits = Translation;
  else if (translationPresent)
    flagBits = Translation | Scale;
  else if (identityAlongDiagonal)
    flagBits = Identity;
  else
    flagBits = Scale;
}
