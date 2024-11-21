#pragma once
#include "MathGlobals.hpp"
#include <assert.h>

template <int N, int M, typename T>
class GenericMatrix {
public:
  GenericMatrix();
  GenericMatrix(const GenericMatrix& other);
  explicit GenericMatrix(const T* values);

  const T& operator()(int row, int column) const;
  T& operator()(int row, int column);

  bool isIdentity() const;
  void setToIdentity();

  void fill(T value);

  GenericMatrix<M, N, T> transposed() const;

  GenericMatrix& operator+=(const GenericMatrix& other);
  GenericMatrix& operator-=(const GenericMatrix& other);
  GenericMatrix& operator*=(T factor);
  GenericMatrix& operator/=(T divisor);
  bool operator==(const GenericMatrix& other) const;
  bool operator!=(const GenericMatrix& other) const;

  void copyDataTo(T* values) const;
  T* data() { return m[0]; }
  const T* data() const { return m[0]; }

  template <int NN, int MM, typename TT>
  friend GenericMatrix<NN, MM, TT> operator+(const GenericMatrix<NN, MM, TT>& m1, const GenericMatrix<NN, MM, TT>& m2);
  template <int NN, int MM, typename TT>
  friend GenericMatrix<NN, MM, TT> operator-(const GenericMatrix<NN, MM, TT>& m1, const GenericMatrix<NN, MM, TT>& m2);
  template <int NN, int M1, int M2, typename TT>
  friend GenericMatrix<M1, M2, TT> operator*(const GenericMatrix<NN, M2, TT>& m1, const GenericMatrix<M1, NN, TT>& m2);
  template <int NN, int MM, typename TT>
  friend GenericMatrix<NN, MM, TT> operator-(const GenericMatrix<NN, MM, TT>& matrix);
  template <int NN, int MM, typename TT>
  friend GenericMatrix<NN, MM, TT> operator*(TT factor, const GenericMatrix<NN, MM, TT>& matrix);
  template <int NN, int MM, typename TT>
  friend GenericMatrix<NN, MM, TT> operator*(const GenericMatrix<NN, MM, TT>& matrix, TT factor);
  template <int NN, int MM, typename TT>
  friend GenericMatrix<NN, MM, TT> operator/(const GenericMatrix<NN, MM, TT>& matrix, TT divisor);

private:
  T m[N][M];

  GenericMatrix(int) {};

  template <int NN, int MM, typename TT>
  friend class GenericMatrix;
};

template <int N, int M, typename T>
GenericMatrix<N, M, T>::GenericMatrix() {
  setToIdentity();
}

template <int N, int M, typename T>
GenericMatrix<N, M, T>::GenericMatrix(const GenericMatrix<N, M, T>& other) {
  for (int col = 0; col < N; ++col)
    for (int row = 0; row < M; ++row)
      m[col][row] = other.m[col][row];
}

template <int N, int M, typename T>
GenericMatrix<N, M, T>::GenericMatrix(const T* values) {
  for (int col = 0; col < N; ++col)
    for (int row = 0; row < M; ++row)
      m[col][row] = values[row * N + col];
}

template <int N, int M, typename T>
const T& GenericMatrix<N, M, T>::operator()(int row, int column) const {
  assert(row >= 0 && row < M && column >= 0 && column < N);
  return m[column][row];
}

template <int N, int M, typename T>
T& GenericMatrix<N, M, T>::operator()(int row, int column) {
  assert(row >= 0 && row < M && column >= 0 && column < N);
  return m[column][row];
}

template <int N, int M, typename T>
bool GenericMatrix<N, M, T>::isIdentity() const {
  for (int col = 0; col < N; ++col) {
    for (int row = 0; row < M; ++row) {
      if (row == col) {
        if (m[col][row] != 1.0f)
          return false;
      } else {
        if (m[col][row] != 0.0f)
          return false;
      }
    }
  }
  return true;
}

template <int N, int M, typename T>
void GenericMatrix<N, M, T>::setToIdentity() {
  for (int col = 0; col < N; ++col) {
    for (int row = 0; row < M; ++row) {
      if (row == col)
        m[col][row] = 1.0f;
      else
        m[col][row] = 0.0f;
    }
  }
}

template <int N, int M, typename T>
void GenericMatrix<N, M, T>::fill(T value) {
  for (int col = 0; col < N; ++col)
    for (int row = 0; row < M; ++row)
      m[col][row] = value;
}

template <int N, int M, typename T>
GenericMatrix<M, N, T> GenericMatrix<N, M, T>::transposed() const {
  GenericMatrix<M, N, T> result(1);
  for (int row = 0; row < M; ++row)
    for (int col = 0; col < N; ++col)
      result.m[row][col] = m[col][row];
  return result;
}

template <int N, int M, typename T>
GenericMatrix<N, M, T>& GenericMatrix<N, M, T>::operator+=(const GenericMatrix<N, M, T>& other) {
  for (int index = 0; index < N * M; ++index)
    m[0][index] += other.m[0][index];
  return *this;
}

template <int N, int M, typename T>
GenericMatrix<N, M, T>& GenericMatrix<N, M, T>::operator-=(const GenericMatrix<N, M, T>& other) {
  for (int index = 0; index < N * M; ++index)
    m[0][index] -= other.m[0][index];
  return *this;
}

template <int N, int M, typename T>
GenericMatrix<N, M, T>& GenericMatrix<N, M, T>::operator*=(T factor) {
  for (int index = 0; index < N * M; ++index)
    m[0][index] *= factor;
  return *this;
}

template <int N, int M, typename T>
bool GenericMatrix<N, M, T>::operator==(const GenericMatrix<N, M, T>& other) const {
  for (int index = 0; index < N * M; ++index) {
    if (m[0][index] != other.m[0][index])
      return false;
  }
  return true;
}

template <int N, int M, typename T>
bool GenericMatrix<N, M, T>::operator!=(const GenericMatrix<N, M, T>& other) const {
  for (int index = 0; index < N * M; ++index) {
    if (m[0][index] != other.m[0][index])
      return true;
  }
  return false;
}

template <int N, int M, typename T>
GenericMatrix<N, M, T>& GenericMatrix<N, M, T>::operator/=(T divisor) {
  for (int index = 0; index < N * M; ++index)
    m[0][index] /= divisor;
  return *this;
}

template <int N, int M, typename T>
GenericMatrix<N, M, T> operator+(const GenericMatrix<N, M, T>& m1, const GenericMatrix<N, M, T>& m2) {
  GenericMatrix<N, M, T> result(1);
  for (int index = 0; index < N * M; ++index)
    result.m[0][index] = m1.m[0][index] + m2.m[0][index];
  return result;
}

template <int N, int M, typename T>
GenericMatrix<N, M, T> operator-(const GenericMatrix<N, M, T>& m1, const GenericMatrix<N, M, T>& m2) {
  GenericMatrix<N, M, T> result(1);
  for (int index = 0; index < N * M; ++index)
    result.m[0][index] = m1.m[0][index] - m2.m[0][index];
  return result;
}

template <int N, int M1, int M2, typename T>
GenericMatrix<M1, M2, T> operator*(const GenericMatrix<N, M2, T>& m1, const GenericMatrix<M1, N, T>& m2) {
  GenericMatrix<M1, M2, T> result(1);
  for (int row = 0; row < M2; ++row) {
    for (int col = 0; col < M1; ++col) {
      T sum(0.0f);
      for (int j = 0; j < N; ++j)
        sum += m1.m[j][row] * m2.m[col][j];
      result.m[col][row] = sum;
    }
  }
  return result;
}

template <int N, int M, typename T>
GenericMatrix<N, M, T> operator-(const GenericMatrix<N, M, T>& matrix) {
  GenericMatrix<N, M, T> result(1);
  for (int index = 0; index < N * M; ++index)
    result.m[0][index] = -matrix.m[0][index];
  return result;
}

template <int N, int M, typename T>
GenericMatrix<N, M, T> operator*(T factor, const GenericMatrix<N, M, T>& matrix) {
  GenericMatrix<N, M, T> result(1);
  for (int index = 0; index < N * M; ++index)
    result.m[0][index] = matrix.m[0][index] * factor;
  return result;
}

template <int N, int M, typename T>
GenericMatrix<N, M, T> operator*(const GenericMatrix<N, M, T>& matrix, T factor) {
  GenericMatrix<N, M, T> result(1);
  for (int index = 0; index < N * M; ++index)
    result.m[0][index] = matrix.m[0][index] * factor;
  return result;
}

template <int N, int M, typename T>
GenericMatrix<N, M, T> operator/(const GenericMatrix<N, M, T>& matrix, T divisor) {
  GenericMatrix<N, M, T> result(1);
  for (int index = 0; index < N * M; ++index)
    result.m[0][index] = matrix.m[0][index] / divisor;
  return result;
}

template <int N, int M, typename T>
void GenericMatrix<N, M, T>::copyDataTo(T* values) const {
  for (int col = 0; col < N; ++col)
    for (int row = 0; row < M; ++row)
      values[row * N + col] = T(m[col][row]);
}

typedef GenericMatrix<2, 2, float> Matrix2x2;
typedef GenericMatrix<2, 3, float> Matrix2x3;
typedef GenericMatrix<2, 4, float> Matrix2x4;
typedef GenericMatrix<3, 2, float> Matrix3x2;
typedef GenericMatrix<3, 3, float> Matrix3x3;
typedef GenericMatrix<3, 4, float> Matrix3x4;
typedef GenericMatrix<4, 2, float> Matrix4x2;
typedef GenericMatrix<4, 3, float> Matrix4x3;