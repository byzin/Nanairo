/*
  \file matrix-inl.h
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_MATRIX_INL_H_
#define _ZISC_MATRIX_INL_H_

#include "matrix.hpp"
// Standard C++ library
#include <utility>
// Zisc
#include "arithmetic_array.hpp"
#include "point.hpp"
#include "utility.hpp"
#include "vector.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

// public member function

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
Matrix<Arithmetic, kRow, kColumn>::Matrix()
{
}

/*!
 \details
 No detailed.
 */
template <typename Arithmetic, uint kRow, uint kColumn>
template <typename ...Types> inline
Matrix<Arithmetic, kRow, kColumn>::Matrix(const Types ...elements) :
    elements_{elements...}
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
bool Matrix<Arithmetic, kRow, kColumn>::operator==(const Matrix& matrix) const
{
  return elements_ == matrix.elements_;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
bool Matrix<Arithmetic, kRow, kColumn>::operator!=(const Matrix& matrix) const
{
  return elements_ != matrix.elements_;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
auto Matrix<Arithmetic, kRow, kColumn>::operator*(const Arithmetic scalar) const
    -> Matrix
{
  Matrix matrix;
  matrix.elements_ = scalar * elements_;
  return matrix;
}

/*!
 \details
 No detailed.
 */
template <typename Arithmetic, uint kRow, uint kColumn> inline
Arithmetic& Matrix<Arithmetic, kRow, kColumn>::operator()(const uint row,
                                                          const uint column)
{
  return elements_[row * columnSize() + column];
}

/*!
 \details
 No detailed.
 */
template <typename Arithmetic, uint kRow, uint kColumn> inline
const Arithmetic& Matrix<Arithmetic, kRow, kColumn>::operator()(const uint row,
                                                                const uint column) const
{
  return elements_[row * columnSize() + column];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
Arithmetic Matrix<Arithmetic, kRow, kColumn>::cofactor(const uint row,
                                                       const uint column) const
{
  const auto determinant = minorDeterminant(row, column);
  return (((row + column) & 1) == 1) ? -determinant : determinant;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
auto Matrix<Arithmetic, kRow, kColumn>::cofactorMatrix() const -> Matrix
{
  Matrix cofactor_matrix;
  for (uint row = 0; row < rowSize(); ++row) {
    for (uint column = 0; column < columnSize(); ++column) {
      cofactor_matrix.set(row, column, cofactor(column, row));
    }
  }
  return cofactor_matrix;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
constexpr uint Matrix<Arithmetic, kRow, kColumn>::columnSize()
{
  return kColumn;
}

namespace zisc_matrix {

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic calculateDeterminant(const Matrix<Arithmetic, kN, kN>& matrix)
{
  Arithmetic determinant = cast<Arithmetic>(0);
  for (uint column = 0; column < kN; ++column)
    determinant += matrix(0, column) * matrix.cofactor(0, column);
  return determinant;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic> inline
Arithmetic calculateDeterminant(const Matrix<Arithmetic, 2, 2>& matrix)
{
  return matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic> inline
Arithmetic calculateDeterminant(const Matrix<Arithmetic, 1, 1>& matrix)
{
  return matrix(0, 0);
}

} // namespace zisc_matrix

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
Arithmetic Matrix<Arithmetic, kRow, kColumn>::determinant() const
{
  static_assert(isSquareMatrix(), "## Matrix must be square matrix.");
  return zisc_matrix::calculateDeterminant(*this);
}

/*!
 \details
 No detailed.
 */
template <typename Arithmetic, uint kRow, uint kColumn> inline
Arithmetic Matrix<Arithmetic, kRow, kColumn>::get(const uint row, 
                                                  const uint column) const
{
  return elements_.get(row * kColumn + column);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
auto Matrix<Arithmetic, kRow, kColumn>::inverseMatrix() const -> Matrix
{
  return (cast<Arithmetic>(1) / determinant()) * cofactorMatrix();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
constexpr bool Matrix<Arithmetic, kRow, kColumn>::isSquareMatrix()
{
  return (kRow == kColumn);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn>
Arithmetic Matrix<Arithmetic, kRow, kColumn>::minorDeterminant(
    const uint row, const uint column) const
{
  static_assert(isSquareMatrix(), "## Matrix must be square matrix.");
  constexpr uint n = rowSize();

  // Make submatrix
  Matrix<Arithmetic, n - 1, n - 1> submatrix;
  uint r = 0,
       c = 0;
  for (uint i = 0; i < rowSize(); ++i) {
    if (i == row)
      continue;
    for (uint j = 0; j < columnSize(); ++j) {
      if (j == column)
        continue;
      submatrix.set(r, c, get(i, j));
      ++c;
    }
    ++r;
    c = 0;
  }

  // Calculate minor determinant
  return submatrix.determinant();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
constexpr uint Matrix<Arithmetic, kRow, kColumn>::rowSize()
{
  return kRow;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
void Matrix<Arithmetic, kRow, kColumn>::set(const uint row,
                                            const uint column,
                                            const Arithmetic value)
{
  elements_.set(row * columnSize() + column, value);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
Matrix<Arithmetic, kColumn, kRow> Matrix<Arithmetic, kRow, kColumn>::
    transposedMatrix() const
{
  Matrix<Arithmetic, kColumn, kRow> transposed_matrix;
  for (uint row = 0; row < rowSize(); ++row) {
    for (uint column = 0; column < columnSize(); ++column) {
      transposed_matrix.set(column, row, get(row, column));
    }
  }
  return transposed_matrix;
}

/*!
 \details
 No detailed.
 */
template <typename Arithmetic, uint L, uint M, uint N>
Matrix<Arithmetic, L, N> operator*(const Matrix<Arithmetic, L, M>& a,
                                   const Matrix<Arithmetic, M, N>& b)
{
  Matrix<Arithmetic, L, N> matrix{};
  Arithmetic value;
  for (uint l = 0; l < L; ++l) {
    for (uint n = 0; n < N; ++n) {
      value = cast<Arithmetic>(0);
      for (uint m = 0; m < M; ++m)
        value += a(l, m) * b(m, n);
      matrix.set(l, n, value);
    }
  }
  return matrix;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
Matrix<Arithmetic, kRow, kColumn> operator*(
    const Arithmetic scalar,
    const Matrix<Arithmetic, kRow, kColumn>& matrix)
{
  return matrix * scalar;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> operator*(
    const Matrix<Arithmetic, kN, kN>& matrix,
    const ArithmeticArray<Arithmetic, kN>& array)
{
  ArithmeticArray<Arithmetic, kN> result;
  Arithmetic value;
  for (uint row = 0; row < kN; ++row) {
    value = cast<Arithmetic>(0);
    for (uint column = 0; column < kN; ++column)
      value += matrix(row, column) * array[column];
    result.set(row, value);
  }
  return result;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Point<Arithmetic, kN> operator*(const Matrix<Arithmetic, kN, kN>& matrix,
                                const Point<Arithmetic, kN>& point)
{
  return Point<Arithmetic, kN>{matrix * point.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator*(const Matrix<Arithmetic, kN, kN>& matrix,
                                 const Vector<Arithmetic, kN>& vector)
{
  return Vector<Arithmetic, kN>{matrix * vector.data()};
}

} // namespace zisc

#endif // _ZISC_MATRIX_INL_H_
