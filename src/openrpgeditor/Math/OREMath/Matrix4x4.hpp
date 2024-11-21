#pragma once
#include "MathGlobals.hpp"
#include "Vector3D.hpp"
#include "Vector4D.hpp"
#include "GenericMatrix.hpp"
#include "Rect.hpp"

class Matrix4x4 {
public:
  Matrix4x4() { setToIdentity(); }
  explicit Matrix4x4(const float* values);
  Matrix4x4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31,
            float m32, float m33, float m34, float m41, float m42, float m43, float m44);
  template <int N, int M>
  explicit Matrix4x4(const GenericMatrix<N, M, float>& matrix);
  Matrix4x4(const float* values, int cols, int rows);

  const float& operator()(int row, int col) const;
  float& operator()(int row, int col);

  [[nodiscard]] Vector4D column(int index) const;
  void setColumn(int index, const Vector4D& value);

  [[nodiscard]] Vector4D row(int index) const;
  void setRow(int index, const Vector4D& value);

  [[nodiscard]] bool isIdentity() const;
  void setToIdentity();

  void fill(float value);

  [[nodiscard]] float determinant() const;
  [[nodiscard]] Matrix4x4 inverted(bool* invertible) const;
  [[nodiscard]] Matrix4x4 transposed() const;
  [[nodiscard]] Matrix3x3 normalMatrix() const;

  inline Matrix4x4& operator+=(const Matrix4x4& other);
  inline Matrix4x4& operator-=(const Matrix4x4& other);
  inline Matrix4x4& operator*=(const Matrix4x4& other);
  inline Matrix4x4& operator*=(float factor);
  Matrix4x4& operator/=(float divisor);
  inline bool operator==(const Matrix4x4& other) const;
  inline bool operator!=(const Matrix4x4& other) const;

  friend Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
  friend Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
  friend Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
  friend Vector3D operator*(const Matrix4x4& matrix, const Vector3D& vector);
  friend Vector3D operator*(const Vector3D& vector, const Matrix4x4& matrix);
  friend Vector4D operator*(const Vector4D& vector, const Matrix4x4& matrix);
  friend Vector4D operator*(const Matrix4x4& matrix, const Vector4D& vector);
  friend Point operator*(const Point& point, const Matrix4x4& matrix);
  friend PointF operator*(const PointF& point, const Matrix4x4& matrix);
  friend Matrix4x4 operator-(const Matrix4x4& matrix);
  friend Point operator*(const Matrix4x4& matrix, const Point& point);
  friend PointF operator*(const Matrix4x4& matrix, const PointF& point);
  friend Matrix4x4 operator*(float factor, const Matrix4x4& matrix);
  friend Matrix4x4 operator*(const Matrix4x4& matrix, float factor);
  friend Matrix4x4 operator/(const Matrix4x4& matrix, float divisor);

  friend inline bool fuzzyCompare(const Matrix4x4& m1, const Matrix4x4& m2);

  void scale(const Vector3D& vector);
  void translate(const Vector3D& vector);
  void rotate(float angle, const Vector3D& vector);
  void scale(float x, float y);
  void scale(float x, float y, float z);
  void scale(float factor);
  void translate(float x, float y);
  void translate(float x, float y, float z);
  void rotate(float angle, float x, float y, float z = 0.0f);
  // void rotate(const Quaternion& quaternion);

  void ortho(const Rect& rect);
  void ortho(const RectF& rect);
  void ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane);
  void frustum(float left, float right, float bottom, float top, float nearPlane, float farPlane);
  void perspective(float angle, float aspect, float nearPlane, float farPlane);
  void lookAt(const Vector3D& eye, const Vector3D& center, const Vector3D& up);
  void flipCoordinates();

  void copyDataTo(float* values) const;

  [[nodiscard]] Point map(const Point& point) const;
  [[nodiscard]] PointF map(const PointF& point) const;
  [[nodiscard]] Vector3D map(const Vector3D& point) const;
  [[nodiscard]] Vector3D mapVector(const Vector3D& vector) const;
  [[nodiscard]] Vector4D map(const Vector4D& point) const;
  [[nodiscard]] Rect mapRect(const Rect& rect) const;
  [[nodiscard]] RectF mapRect(const RectF& rect) const;

  template <int N, int M>
  GenericMatrix<N, M, float> toGenericMatrix() const;

  float* data();
  [[nodiscard]] const float* data() const { return m[0]; }

  void optimize();

private:
  float m[4][4]{};
  int flagBits{};
  enum {
    Identity = 1 << 0,
    General = 1 << 1,
    Translation = 1 << 2,
    Scale = 1 << 3,
    Rotation = 1 << 4,
  };

  explicit Matrix4x4(int) { flagBits = General; }

  [[nodiscard]] Matrix4x4 orthonormalInverse() const;
  void projectedRotate(float angle, float x, float y, float z);
};

inline Matrix4x4::Matrix4x4(const float m11, const float m12, const float m13, const float m14, const float m21,
                            const float m22, const float m23, const float m24, const float m31, const float m32,
                            const float m33, const float m34, const float m41, const float m42, const float m43,
                            const float m44) {
  m[0][0] = m11;
  m[0][1] = m21;
  m[0][2] = m31;
  m[0][3] = m41;
  m[1][0] = m12;
  m[1][1] = m22;
  m[1][2] = m32;
  m[1][3] = m42;
  m[2][0] = m13;
  m[2][1] = m23;
  m[2][2] = m33;
  m[2][3] = m43;
  m[3][0] = m14;
  m[3][1] = m24;
  m[3][2] = m34;
  m[3][3] = m44;
  flagBits = General;
}
template <int N, int M>
Matrix4x4::Matrix4x4(const GenericMatrix<N, M, float>& matrix) {
  const float* values = matrix.data();
  for (int matrixCol = 0; matrixCol < 4; ++matrixCol) {
    for (int matrixRow = 0; matrixRow < 4; ++matrixRow) {
      if (matrixCol < N && matrixRow < M)
        m[matrixCol][matrixRow] = values[matrixCol * M + matrixRow];
      else if (matrixCol == matrixRow)
        m[matrixCol][matrixRow] = 1.0f;
      else
        m[matrixCol][matrixRow] = 0.0f;
    }
  }
  flagBits = General;
}
template <int N, int M>
GenericMatrix<N, M, float> Matrix4x4::toGenericMatrix() const {
  GenericMatrix<N, M, float> result;
  float* values = result.data();
  for (int matrixCol = 0; matrixCol < N; ++matrixCol) {
    for (int matrixRow = 0; matrixRow < M; ++matrixRow) {
      if (matrixCol < 4 && matrixRow < 4)
        values[matrixCol * M + matrixRow] = m[matrixCol][matrixRow];
      else if (matrixCol == matrixRow)
        values[matrixCol * M + matrixRow] = 1.0f;
      else
        values[matrixCol * M + matrixRow] = 0.0f;
    }
  }
  return result;
}

inline const float& Matrix4x4::operator()(const int row, const int col) const {
  assert(row >= 0 && row < 4 && col >= 0 && col < 4);
  return m[col][row];
}

inline float& Matrix4x4::operator()(const int row, const int col) {
  assert(row >= 0 && row < 4 && col >= 0 && col < 4);
  flagBits = General;
  return m[col][row];
}

inline Vector4D Matrix4x4::column(const int index) const {
  assert(index >= 0 && index < 4);
  return {m[index][0], m[index][1], m[index][2], m[index][3]};
}

inline void Matrix4x4::setColumn(const int index, const Vector4D& value) {
  assert(index >= 0 && index < 4);
  m[index][0] = value.x();
  m[index][1] = value.y();
  m[index][2] = value.z();
  m[index][3] = value.w();
  flagBits = General;
}

inline Vector4D Matrix4x4::row(const int index) const {
  assert(index >= 0 && index < 4);
  return {m[0][index], m[1][index], m[2][index], m[3][index]};
}

inline void Matrix4x4::setRow(const int index, const Vector4D& value) {
  assert(index >= 0 && index < 4);
  m[0][index] = value.x();
  m[1][index] = value.y();
  m[2][index] = value.z();
  m[3][index] = value.w();
  flagBits = General;
}

Matrix4x4 operator/(const Matrix4x4& matrix, float divisor);

inline bool Matrix4x4::isIdentity() const {
  if (flagBits == Identity)
    return true;
  if (m[0][0] != 1.0f || m[0][1] != 0.0f || m[0][2] != 0.0f)
    return false;
  if (m[0][3] != 0.0f || m[1][0] != 0.0f || m[1][1] != 1.0f)
    return false;
  if (m[1][2] != 0.0f || m[1][3] != 0.0f || m[2][0] != 0.0f)
    return false;
  if (m[2][1] != 0.0f || m[2][2] != 1.0f || m[2][3] != 0.0f)
    return false;
  if (m[3][0] != 0.0f || m[3][1] != 0.0f || m[3][2] != 0.0f)
    return false;
  return m[3][3] == 1.0f;
}

inline void Matrix4x4::setToIdentity() {
  m[0][0] = 1.0f;
  m[0][1] = 0.0f;
  m[0][2] = 0.0f;
  m[0][3] = 0.0f;
  m[1][0] = 0.0f;
  m[1][1] = 1.0f;
  m[1][2] = 0.0f;
  m[1][3] = 0.0f;
  m[2][0] = 0.0f;
  m[2][1] = 0.0f;
  m[2][2] = 1.0f;
  m[2][3] = 0.0f;
  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
  flagBits = Identity;
}

inline void Matrix4x4::fill(const float value) {
  m[0][0] = value;
  m[0][1] = value;
  m[0][2] = value;
  m[0][3] = value;
  m[1][0] = value;
  m[1][1] = value;
  m[1][2] = value;
  m[1][3] = value;
  m[2][0] = value;
  m[2][1] = value;
  m[2][2] = value;
  m[2][3] = value;
  m[3][0] = value;
  m[3][1] = value;
  m[3][2] = value;
  m[3][3] = value;
  flagBits = General;
}

inline Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other) {
  m[0][0] += other.m[0][0];
  m[0][1] += other.m[0][1];
  m[0][2] += other.m[0][2];
  m[0][3] += other.m[0][3];
  m[1][0] += other.m[1][0];
  m[1][1] += other.m[1][1];
  m[1][2] += other.m[1][2];
  m[1][3] += other.m[1][3];
  m[2][0] += other.m[2][0];
  m[2][1] += other.m[2][1];
  m[2][2] += other.m[2][2];
  m[2][3] += other.m[2][3];
  m[3][0] += other.m[3][0];
  m[3][1] += other.m[3][1];
  m[3][2] += other.m[3][2];
  m[3][3] += other.m[3][3];
  flagBits = General;
  return *this;
}

inline Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other) {
  m[0][0] -= other.m[0][0];
  m[0][1] -= other.m[0][1];
  m[0][2] -= other.m[0][2];
  m[0][3] -= other.m[0][3];
  m[1][0] -= other.m[1][0];
  m[1][1] -= other.m[1][1];
  m[1][2] -= other.m[1][2];
  m[1][3] -= other.m[1][3];
  m[2][0] -= other.m[2][0];
  m[2][1] -= other.m[2][1];
  m[2][2] -= other.m[2][2];
  m[2][3] -= other.m[2][3];
  m[3][0] -= other.m[3][0];
  m[3][1] -= other.m[3][1];
  m[3][2] -= other.m[3][2];
  m[3][3] -= other.m[3][3];
  flagBits = General;
  return *this;
}

inline Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other) {
  if (flagBits == Identity) {
    *this = other;
    return *this;
  }
  if (other.flagBits == Identity) {
    return *this;
  }
  *this = *this * other;
  return *this;
}

inline Matrix4x4& Matrix4x4::operator*=(const float factor) {
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
  m[3][0] *= factor;
  m[3][1] *= factor;
  m[3][2] *= factor;
  m[3][3] *= factor;
  flagBits = General;
  return *this;
}

inline bool Matrix4x4::operator==(const Matrix4x4& other) const {
  return m[0][0] == other.m[0][0] && m[0][1] == other.m[0][1] && m[0][2] == other.m[0][2] && m[0][3] == other.m[0][3] &&
         m[1][0] == other.m[1][0] && m[1][1] == other.m[1][1] && m[1][2] == other.m[1][2] && m[1][3] == other.m[1][3] &&
         m[2][0] == other.m[2][0] && m[2][1] == other.m[2][1] && m[2][2] == other.m[2][2] && m[2][3] == other.m[2][3] &&
         m[3][0] == other.m[3][0] && m[3][1] == other.m[3][1] && m[3][2] == other.m[3][2] && m[3][3] == other.m[3][3];
}

inline bool Matrix4x4::operator!=(const Matrix4x4& other) const {
  return m[0][0] != other.m[0][0] || m[0][1] != other.m[0][1] || m[0][2] != other.m[0][2] || m[0][3] != other.m[0][3] ||
         m[1][0] != other.m[1][0] || m[1][1] != other.m[1][1] || m[1][2] != other.m[1][2] || m[1][3] != other.m[1][3] ||
         m[2][0] != other.m[2][0] || m[2][1] != other.m[2][1] || m[2][2] != other.m[2][2] || m[2][3] != other.m[2][3] ||
         m[3][0] != other.m[3][0] || m[3][1] != other.m[3][1] || m[3][2] != other.m[3][2] || m[3][3] != other.m[3][3];
}

inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
  Matrix4x4 m(1);
  m.m[0][0] = m1.m[0][0] + m2.m[0][0];
  m.m[0][1] = m1.m[0][1] + m2.m[0][1];
  m.m[0][2] = m1.m[0][2] + m2.m[0][2];
  m.m[0][3] = m1.m[0][3] + m2.m[0][3];
  m.m[1][0] = m1.m[1][0] + m2.m[1][0];
  m.m[1][1] = m1.m[1][1] + m2.m[1][1];
  m.m[1][2] = m1.m[1][2] + m2.m[1][2];
  m.m[1][3] = m1.m[1][3] + m2.m[1][3];
  m.m[2][0] = m1.m[2][0] + m2.m[2][0];
  m.m[2][1] = m1.m[2][1] + m2.m[2][1];
  m.m[2][2] = m1.m[2][2] + m2.m[2][2];
  m.m[2][3] = m1.m[2][3] + m2.m[2][3];
  m.m[3][0] = m1.m[3][0] + m2.m[3][0];
  m.m[3][1] = m1.m[3][1] + m2.m[3][1];
  m.m[3][2] = m1.m[3][2] + m2.m[3][2];
  m.m[3][3] = m1.m[3][3] + m2.m[3][3];
  return m;
}

inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
  Matrix4x4 m(1);
  m.m[0][0] = m1.m[0][0] - m2.m[0][0];
  m.m[0][1] = m1.m[0][1] - m2.m[0][1];
  m.m[0][2] = m1.m[0][2] - m2.m[0][2];
  m.m[0][3] = m1.m[0][3] - m2.m[0][3];
  m.m[1][0] = m1.m[1][0] - m2.m[1][0];
  m.m[1][1] = m1.m[1][1] - m2.m[1][1];
  m.m[1][2] = m1.m[1][2] - m2.m[1][2];
  m.m[1][3] = m1.m[1][3] - m2.m[1][3];
  m.m[2][0] = m1.m[2][0] - m2.m[2][0];
  m.m[2][1] = m1.m[2][1] - m2.m[2][1];
  m.m[2][2] = m1.m[2][2] - m2.m[2][2];
  m.m[2][3] = m1.m[2][3] - m2.m[2][3];
  m.m[3][0] = m1.m[3][0] - m2.m[3][0];
  m.m[3][1] = m1.m[3][1] - m2.m[3][1];
  m.m[3][2] = m1.m[3][2] - m2.m[3][2];
  m.m[3][3] = m1.m[3][3] - m2.m[3][3];
  return m;
}

inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
  if (m1.flagBits == Matrix4x4::Identity) {
    return m2;
  }
  if (m2.flagBits == Matrix4x4::Identity) {
    return m1;
  }

  Matrix4x4 m(1);
  m.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[1][0] * m2.m[0][1] + m1.m[2][0] * m2.m[0][2] + m1.m[3][0] * m2.m[0][3];
  m.m[0][1] = m1.m[0][1] * m2.m[0][0] + m1.m[1][1] * m2.m[0][1] + m1.m[2][1] * m2.m[0][2] + m1.m[3][1] * m2.m[0][3];
  m.m[0][2] = m1.m[0][2] * m2.m[0][0] + m1.m[1][2] * m2.m[0][1] + m1.m[2][2] * m2.m[0][2] + m1.m[3][2] * m2.m[0][3];
  m.m[0][3] = m1.m[0][3] * m2.m[0][0] + m1.m[1][3] * m2.m[0][1] + m1.m[2][3] * m2.m[0][2] + m1.m[3][3] * m2.m[0][3];
  m.m[1][0] = m1.m[0][0] * m2.m[1][0] + m1.m[1][0] * m2.m[1][1] + m1.m[2][0] * m2.m[1][2] + m1.m[3][0] * m2.m[1][3];
  m.m[1][1] = m1.m[0][1] * m2.m[1][0] + m1.m[1][1] * m2.m[1][1] + m1.m[2][1] * m2.m[1][2] + m1.m[3][1] * m2.m[1][3];
  m.m[1][2] = m1.m[0][2] * m2.m[1][0] + m1.m[1][2] * m2.m[1][1] + m1.m[2][2] * m2.m[1][2] + m1.m[3][2] * m2.m[1][3];
  m.m[1][3] = m1.m[0][3] * m2.m[1][0] + m1.m[1][3] * m2.m[1][1] + m1.m[2][3] * m2.m[1][2] + m1.m[3][3] * m2.m[1][3];
  m.m[2][0] = m1.m[0][0] * m2.m[2][0] + m1.m[1][0] * m2.m[2][1] + m1.m[2][0] * m2.m[2][2] + m1.m[3][0] * m2.m[2][3];
  m.m[2][1] = m1.m[0][1] * m2.m[2][0] + m1.m[1][1] * m2.m[2][1] + m1.m[2][1] * m2.m[2][2] + m1.m[3][1] * m2.m[2][3];
  m.m[2][2] = m1.m[0][2] * m2.m[2][0] + m1.m[1][2] * m2.m[2][1] + m1.m[2][2] * m2.m[2][2] + m1.m[3][2] * m2.m[2][3];
  m.m[2][3] = m1.m[0][3] * m2.m[2][0] + m1.m[1][3] * m2.m[2][1] + m1.m[2][3] * m2.m[2][2] + m1.m[3][3] * m2.m[2][3];
  m.m[3][0] = m1.m[0][0] * m2.m[3][0] + m1.m[1][0] * m2.m[3][1] + m1.m[2][0] * m2.m[3][2] + m1.m[3][0] * m2.m[3][3];
  m.m[3][1] = m1.m[0][1] * m2.m[3][0] + m1.m[1][1] * m2.m[3][1] + m1.m[2][1] * m2.m[3][2] + m1.m[3][1] * m2.m[3][3];
  m.m[3][2] = m1.m[0][2] * m2.m[3][0] + m1.m[1][2] * m2.m[3][1] + m1.m[2][2] * m2.m[3][2] + m1.m[3][2] * m2.m[3][3];
  m.m[3][3] = m1.m[0][3] * m2.m[3][0] + m1.m[1][3] * m2.m[3][1] + m1.m[2][3] * m2.m[3][2] + m1.m[3][3] * m2.m[3][3];
  return m;
}

inline Vector3D operator*(const Vector3D& vector, const Matrix4x4& matrix) {
  const float x =
      vector.x() * matrix.m[0][0] + vector.y() * matrix.m[0][1] + vector.z() * matrix.m[0][2] + matrix.m[0][3];
  const float y =
      vector.x() * matrix.m[1][0] + vector.y() * matrix.m[1][1] + vector.z() * matrix.m[1][2] + matrix.m[1][3];
  const float z =
      vector.x() * matrix.m[2][0] + vector.y() * matrix.m[2][1] + vector.z() * matrix.m[2][2] + matrix.m[2][3];
  const float w =
      vector.x() * matrix.m[3][0] + vector.y() * matrix.m[3][1] + vector.z() * matrix.m[3][2] + matrix.m[3][3];
  if (w == 1.0f) {
    return {x, y, z};
  }
  return {x / w, y / w, z / w};
}

inline Vector3D operator*(const Matrix4x4& matrix, const Vector3D& vector) {
  if (matrix.flagBits == Matrix4x4::Identity) {
    return vector;
  }
  if (matrix.flagBits == Matrix4x4::Translation) {
    return {vector.x() + matrix.m[3][0], vector.y() + matrix.m[3][1], vector.z() + matrix.m[3][2]};
  }
  if (matrix.flagBits == (Matrix4x4::Translation | Matrix4x4::Scale)) {
    return {vector.x() * matrix.m[0][0] + matrix.m[3][0], vector.y() * matrix.m[1][1] + matrix.m[3][1],
            vector.z() * matrix.m[2][2] + matrix.m[3][2]};
  }
  if (matrix.flagBits == Matrix4x4::Scale) {
    return {vector.x() * matrix.m[0][0], vector.y() * matrix.m[1][1], vector.z() * matrix.m[2][2]};
  }
  const float x =
      vector.x() * matrix.m[0][0] + vector.y() * matrix.m[1][0] + vector.z() * matrix.m[2][0] + matrix.m[3][0];
  const float y =
      vector.x() * matrix.m[0][1] + vector.y() * matrix.m[1][1] + vector.z() * matrix.m[2][1] + matrix.m[3][1];
  const float z =
      vector.x() * matrix.m[0][2] + vector.y() * matrix.m[1][2] + vector.z() * matrix.m[2][2] + matrix.m[3][2];
  const float w =
      vector.x() * matrix.m[0][3] + vector.y() * matrix.m[1][3] + vector.z() * matrix.m[2][3] + matrix.m[3][3];
  if (w == 1.0f) {
    return {x, y, z};
  }
  return {x / w, y / w, z / w};
}

inline Vector4D operator*(const Vector4D& vector, const Matrix4x4& matrix) {
  const auto x = vector.x() * matrix.m[0][0] + vector.y() * matrix.m[0][1] + vector.z() * matrix.m[0][2] +
                 vector.w() * matrix.m[0][3];
  const auto y = vector.x() * matrix.m[1][0] + vector.y() * matrix.m[1][1] + vector.z() * matrix.m[1][2] +
                 vector.w() * matrix.m[1][3];
  const auto z = vector.x() * matrix.m[2][0] + vector.y() * matrix.m[2][1] + vector.z() * matrix.m[2][2] +
                 vector.w() * matrix.m[2][3];
  const auto w = vector.x() * matrix.m[3][0] + vector.y() * matrix.m[3][1] + vector.z() * matrix.m[3][2] +
                 vector.w() * matrix.m[3][3];
  return {x, y, z, w};
}

inline Vector4D operator*(const Matrix4x4& matrix, const Vector4D& vector) {
  const float x = vector.x() * matrix.m[0][0] + vector.y() * matrix.m[1][0] + vector.z() * matrix.m[2][0] +
                  vector.w() * matrix.m[3][0];
  const float y = vector.x() * matrix.m[0][1] + vector.y() * matrix.m[1][1] + vector.z() * matrix.m[2][1] +
                  vector.w() * matrix.m[3][1];
  const float z = vector.x() * matrix.m[0][2] + vector.y() * matrix.m[1][2] + vector.z() * matrix.m[2][2] +
                  vector.w() * matrix.m[3][2];
  const float w = vector.x() * matrix.m[0][3] + vector.y() * matrix.m[1][3] + vector.z() * matrix.m[2][3] +
                  vector.w() * matrix.m[3][3];
  return {x, y, z, w};
}

inline Point operator*(const Point& point, const Matrix4x4& matrix) {
  const auto xin = static_cast<float>(point.x());
  const auto yin = static_cast<float>(point.y());
  const auto x = xin * matrix.m[0][0] + yin * matrix.m[0][1] + matrix.m[0][3];
  const auto y = xin * matrix.m[1][0] + yin * matrix.m[1][1] + matrix.m[1][3];
  const auto w = xin * matrix.m[3][0] + yin * matrix.m[3][1] + matrix.m[3][3];
  if (w == 1.0f) {
    return {oRound(x), oRound(y)};
  }
  return {oRound(x / w), oRound(y / w)};
}

inline PointF operator*(const PointF& point, const Matrix4x4& matrix) {
  const auto xin = point.x();
  const auto yin = point.y();
  const auto x = xin * matrix.m[0][0] + yin * matrix.m[0][1] + matrix.m[0][3];
  const auto y = xin * matrix.m[1][0] + yin * matrix.m[1][1] + matrix.m[1][3];
  const auto w = xin * matrix.m[3][0] + yin * matrix.m[3][1] + matrix.m[3][3];
  if (w == 1.0f) {
    return {x, y};
  }

  return {x / w, y / w};
}

inline Point operator*(const Matrix4x4& matrix, const Point& point) {
  const auto xin = static_cast<float>(point.x());
  const auto yin = static_cast<float>(point.y());
  if (matrix.flagBits == Matrix4x4::Identity) {
    return point;
  }
  if (matrix.flagBits == Matrix4x4::Translation) {
    return {oRound(xin + matrix.m[3][0]), oRound(yin + matrix.m[3][1])};
  }
  if (matrix.flagBits == (Matrix4x4::Translation | Matrix4x4::Scale)) {
    return {oRound(xin * matrix.m[0][0] + matrix.m[3][0]), oRound(yin * matrix.m[1][1] + matrix.m[3][1])};
  }
  if (matrix.flagBits == Matrix4x4::Scale) {
    return {oRound(xin * matrix.m[0][0]), oRound(yin * matrix.m[1][1])};
  }

  const auto x = xin * matrix.m[0][0] + yin * matrix.m[1][0] + matrix.m[3][0];
  const auto y = xin * matrix.m[0][1] + yin * matrix.m[1][1] + matrix.m[3][1];
  const auto w = xin * matrix.m[0][3] + yin * matrix.m[1][3] + matrix.m[3][3];
  if (w == 1.0f) {
    return {oRound(x), oRound(y)};
  }
  return {oRound(x / w), oRound(y / w)};
}

inline PointF operator*(const Matrix4x4& matrix, const PointF& point) {
  const auto xin = point.x();
  const auto yin = point.y();
  if (matrix.flagBits == Matrix4x4::Identity) {
    return point;
  }
  if (matrix.flagBits == Matrix4x4::Translation) {
    return {xin + matrix.m[3][0], yin + matrix.m[3][1]};
  }
  if (matrix.flagBits == (Matrix4x4::Translation | Matrix4x4::Scale)) {
    return {xin * matrix.m[0][0] + matrix.m[3][0], yin * matrix.m[1][1] + matrix.m[3][1]};
  }
  if (matrix.flagBits == Matrix4x4::Scale) {
    return {xin * matrix.m[0][0], yin * matrix.m[1][1]};
  }
  const auto x = xin * matrix.m[0][0] + yin * matrix.m[1][0] + matrix.m[3][0];
  const auto y = xin * matrix.m[0][1] + yin * matrix.m[1][1] + matrix.m[3][1];
  const auto w = xin * matrix.m[0][3] + yin * matrix.m[1][3] + matrix.m[3][3];
  if (w == 1.0f) {
    return {x, y};
  }
  return {x / w, y / w};
}

inline Matrix4x4 operator-(const Matrix4x4& matrix) {
  Matrix4x4 m(1);
  m.m[0][0] = -matrix.m[0][0];
  m.m[0][1] = -matrix.m[0][1];
  m.m[0][2] = -matrix.m[0][2];
  m.m[0][3] = -matrix.m[0][3];
  m.m[1][0] = -matrix.m[1][0];
  m.m[1][1] = -matrix.m[1][1];
  m.m[1][2] = -matrix.m[1][2];
  m.m[1][3] = -matrix.m[1][3];
  m.m[2][0] = -matrix.m[2][0];
  m.m[2][1] = -matrix.m[2][1];
  m.m[2][2] = -matrix.m[2][2];
  m.m[2][3] = -matrix.m[2][3];
  m.m[3][0] = -matrix.m[3][0];
  m.m[3][1] = -matrix.m[3][1];
  m.m[3][2] = -matrix.m[3][2];
  m.m[3][3] = -matrix.m[3][3];
  return m;
}

inline Matrix4x4 operator*(const float factor, const Matrix4x4& matrix) {
  Matrix4x4 m(1);
  m.m[0][0] = matrix.m[0][0] * factor;
  m.m[0][1] = matrix.m[0][1] * factor;
  m.m[0][2] = matrix.m[0][2] * factor;
  m.m[0][3] = matrix.m[0][3] * factor;
  m.m[1][0] = matrix.m[1][0] * factor;
  m.m[1][1] = matrix.m[1][1] * factor;
  m.m[1][2] = matrix.m[1][2] * factor;
  m.m[1][3] = matrix.m[1][3] * factor;
  m.m[2][0] = matrix.m[2][0] * factor;
  m.m[2][1] = matrix.m[2][1] * factor;
  m.m[2][2] = matrix.m[2][2] * factor;
  m.m[2][3] = matrix.m[2][3] * factor;
  m.m[3][0] = matrix.m[3][0] * factor;
  m.m[3][1] = matrix.m[3][1] * factor;
  m.m[3][2] = matrix.m[3][2] * factor;
  m.m[3][3] = matrix.m[3][3] * factor;
  return m;
}

inline Matrix4x4 operator*(const Matrix4x4& matrix, const float factor) {
  Matrix4x4 m(1);
  m.m[0][0] = matrix.m[0][0] * factor;
  m.m[0][1] = matrix.m[0][1] * factor;
  m.m[0][2] = matrix.m[0][2] * factor;
  m.m[0][3] = matrix.m[0][3] * factor;
  m.m[1][0] = matrix.m[1][0] * factor;
  m.m[1][1] = matrix.m[1][1] * factor;
  m.m[1][2] = matrix.m[1][2] * factor;
  m.m[1][3] = matrix.m[1][3] * factor;
  m.m[2][0] = matrix.m[2][0] * factor;
  m.m[2][1] = matrix.m[2][1] * factor;
  m.m[2][2] = matrix.m[2][2] * factor;
  m.m[2][3] = matrix.m[2][3] * factor;
  m.m[3][0] = matrix.m[3][0] * factor;
  m.m[3][1] = matrix.m[3][1] * factor;
  m.m[3][2] = matrix.m[3][2] * factor;
  m.m[3][3] = matrix.m[3][3] * factor;
  return m;
}

inline bool fuzzyCompare(const Matrix4x4& m1, const Matrix4x4& m2) {
  return fuzzyCompare(m1.m[0][0], m2.m[0][0]) && fuzzyCompare(m1.m[0][1], m2.m[0][1]) &&
         fuzzyCompare(m1.m[0][2], m2.m[0][2]) && fuzzyCompare(m1.m[0][3], m2.m[0][3]) &&
         fuzzyCompare(m1.m[1][0], m2.m[1][0]) && fuzzyCompare(m1.m[1][1], m2.m[1][1]) &&
         fuzzyCompare(m1.m[1][2], m2.m[1][2]) && fuzzyCompare(m1.m[1][3], m2.m[1][3]) &&
         fuzzyCompare(m1.m[2][0], m2.m[2][0]) && fuzzyCompare(m1.m[2][1], m2.m[2][1]) &&
         fuzzyCompare(m1.m[2][2], m2.m[2][2]) && fuzzyCompare(m1.m[2][3], m2.m[2][3]) &&
         fuzzyCompare(m1.m[3][0], m2.m[3][0]) && fuzzyCompare(m1.m[3][1], m2.m[3][1]) &&
         fuzzyCompare(m1.m[3][2], m2.m[3][2]) && fuzzyCompare(m1.m[3][3], m2.m[3][3]);
}

inline Point Matrix4x4::map(const Point& point) const { return *this * point; }

inline PointF Matrix4x4::map(const PointF& point) const { return *this * point; }

inline Vector3D Matrix4x4::map(const Vector3D& point) const { return *this * point; }

inline Vector3D Matrix4x4::mapVector(const Vector3D& vector) const {
  if (flagBits == Identity || flagBits == Translation) {
    return vector;
  }
  if (flagBits == Scale || flagBits == (Translation | Scale)) {
    return {vector.x() * m[0][0], vector.y() * m[1][1], vector.z() * m[2][2]};
  }
  return {vector.x() * m[0][0] + vector.y() * m[1][0] + vector.z() * m[2][0],
          vector.x() * m[0][1] + vector.y() * m[1][1] + vector.z() * m[2][1],
          vector.x() * m[0][2] + vector.y() * m[1][2] + vector.z() * m[2][2]};
}

inline Vector4D Matrix4x4::map(const Vector4D& point) const { return *this * point; }

inline float* Matrix4x4::data() {
  flagBits = General;
  return m[0];
}