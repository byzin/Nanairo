/*
  \file matrix-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_MATRIX_INL_HPP
#define ZISC_MATRIX_INL_HPP

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
Matrix<Arithmetic, kRow, kColumn>::Matrix() noexcept
{
}

/*!
 \details
 No detailed.
 */
template <typename Arithmetic, uint kRow, uint kColumn>
template <typename ...Types> inline
Matrix<Arithmetic, kRow, kColumn>::Matrix(const Types ...elements) noexcept :
    elements_{elements...}
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
bool Matrix<Arithmetic, kRow, kColumn>::operator==(const Matrix& matrix) const 
    noexcept
{
  return (elements_ == matrix.elements_);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
bool Matrix<Arithmetic, kRow, kColumn>::operator!=(const Matrix& matrix) const
    noexcept
{
  return (elements_ != matrix.elements_);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
auto Matrix<Arithmetic, kRow, kColumn>::operator*(const Arithmetic scalar) const
    noexcept -> Matrix
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
Arithmetic& Matrix<Arithmetic, kRow, kColumn>::operator()(
    const uint row,
    const uint column) noexcept
{
  ZISC_ASSERTION_STATEMENT(constexpr auto zero = cast<uint>(0));
  ZISC_ASSERT(isInBounds(row, zero, rowSize()), "The row is out of range.");
  ZISC_ASSERT(isInBounds(column, zero, columnSize()), "The column is out of range.");
  return get(row, column);
}

/*!
 \details
 No detailed.
 */
template <typename Arithmetic, uint kRow, uint kColumn> inline
const Arithmetic& Matrix<Arithmetic, kRow, kColumn>::operator()(
    const uint row,
    const uint column) const noexcept
{
  ZISC_ASSERTION_STATEMENT(constexpr auto zero = cast<uint>(0));
  ZISC_ASSERT(isInBounds(row, zero, rowSize()), "The row is out of range.");
  ZISC_ASSERT(isInBounds(column, zero, columnSize()), "The column is out of range.");
  return get(row, column);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
Arithmetic Matrix<Arithmetic, kRow, kColumn>::cofactor(
    const uint row,
    const uint column) const noexcept
{
  ZISC_ASSERTION_STATEMENT(constexpr auto zero = cast<uint>(0));
  ZISC_ASSERT(isInBounds(row, zero, rowSize()), "The row is out of range.");
  ZISC_ASSERT(isInBounds(column, zero, columnSize()), "The column is out of range.");
  const auto determinant = minorDeterminant(row, column);
  return (isOdd(row + column)) ? -determinant : determinant;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
auto Matrix<Arithmetic, kRow, kColumn>::cofactorMatrix() const noexcept -> Matrix
{
  Matrix cofactor_matrix;
  for (uint row = 0; row < rowSize(); ++row) {
    for (uint column = 0; column < columnSize(); ++column) {
      cofactor_matrix.set(row, column, cofactor(row, column));
    }
  }
  return cofactor_matrix;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
constexpr uint Matrix<Arithmetic, kRow, kColumn>::columnSize() noexcept
{
  return kColumn;
}

namespace zisc_matrix {

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic calculateDeterminant(const Matrix<Arithmetic, kN, kN>& matrix) noexcept
{
  Arithmetic determinant = cast<Arithmetic>(0);
  for (uint column = 0; column < matrix.columnSize(); ++column)
    determinant += matrix(0, column) * matrix.cofactor(0, column);
  return determinant;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic> inline
Arithmetic calculateDeterminant(const Matrix<Arithmetic, 2, 2>& matrix) noexcept
{
  return matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic> inline
Arithmetic calculateDeterminant(const Matrix<Arithmetic, 1, 1>& matrix) noexcept
{
  return matrix(0, 0);
}

} // namespace zisc_matrix

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
Arithmetic Matrix<Arithmetic, kRow, kColumn>::determinant() const noexcept
{
  static_assert(isSquareMatrix(), "Matrix isn't square matrix.");
  return zisc_matrix::calculateDeterminant(*this);
}

/*!
 \details
 No detailed.
 */
template <typename Arithmetic, uint kRow, uint kColumn> inline
Arithmetic& Matrix<Arithmetic, kRow, kColumn>::get(const uint row, 
                                                   const uint column) noexcept
{
  ZISC_ASSERTION_STATEMENT(constexpr auto zero = cast<uint>(0));
  ZISC_ASSERT(isInBounds(row, zero, rowSize()), "The row is out of range.");
  ZISC_ASSERT(isInBounds(column, zero, columnSize()), "The column is out of range.");
  return elements_.get(row * kColumn + column);
}

/*!
 \details
 No detailed.
 */
template <typename Arithmetic, uint kRow, uint kColumn> inline
const Arithmetic& Matrix<Arithmetic, kRow, kColumn>::get(
    const uint row, 
    const uint column) const noexcept
{
  ZISC_ASSERTION_STATEMENT(constexpr auto zero = cast<uint>(0));
  ZISC_ASSERT(isInBounds(row, zero, rowSize()), "The row is out of range.");
  ZISC_ASSERT(isInBounds(column, zero, columnSize()), "The column is out of range.");
  return elements_.get(row * kColumn + column);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
auto Matrix<Arithmetic, kRow, kColumn>::inverseMatrix() const noexcept -> Matrix
{
  // Check the determinant
  const auto d = determinant();
  ZISC_ASSERTION_STATEMENT(constexpr auto zero = cast<Arithmetic>(0));
  ZISC_ASSERT(d != zero, "The determinant of the matrix is zero.");
  // Get a scaler
  constexpr auto one = cast<Arithmetic>(1);
  const auto k = one / d;
  // Get the inverse matrix
  Matrix inverse_matrix;
  for (uint row = 0; row < rowSize(); ++row) {
    for (uint column = 0; column < columnSize(); ++column) {
      inverse_matrix.set(row, column, k * cofactor(column, row));
    }
  }
  return inverse_matrix;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
constexpr bool Matrix<Arithmetic, kRow, kColumn>::isSquareMatrix() noexcept
{
  return (kRow == kColumn);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn>
Arithmetic Matrix<Arithmetic, kRow, kColumn>::minorDeterminant(
    const uint row, 
    const uint column) const noexcept
{
  static_assert(isSquareMatrix(), "Matrix isn't square matrix.");
  ZISC_ASSERTION_STATEMENT(constexpr auto zero = cast<uint>(0));
  ZISC_ASSERT(isInBounds(row, zero, rowSize()), "The row is out of range.");
  ZISC_ASSERT(isInBounds(column, zero, columnSize()), "The column is out of range.");

  // Make submatrix
  Matrix<Arithmetic, rowSize() - 1, columnSize() - 1> submatrix;
  for (uint i = 0, r = 0; i < rowSize(); ++i) {
    if (i == row)
      continue;
    for (uint j = 0, c = 0; j < columnSize(); ++j) {
      if (j == column)
        continue;
      submatrix.set(r, c, get(i, j));
      ++c;
    }
    ++r;
  }
  // Calculate minor determinant
  return submatrix.determinant();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
constexpr uint Matrix<Arithmetic, kRow, kColumn>::rowSize() noexcept
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
                                            const Arithmetic value) noexcept
{
  elements_.set(row * columnSize() + column, value);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn> inline
Matrix<Arithmetic, kColumn, kRow> Matrix<Arithmetic, kRow, kColumn>::
    transposedMatrix() const noexcept
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
                                   const Matrix<Arithmetic, M, N>& b) noexcept
{
  Matrix<Arithmetic, L, N> matrix;
  for (uint l = 0; l < L; ++l) {
    for (uint n = 0; n < N; ++n) {
      Arithmetic value = cast<Arithmetic>(0);
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
    const Matrix<Arithmetic, kRow, kColumn>& matrix) noexcept
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
    const ArithmeticArray<Arithmetic, kN>& array) noexcept
{
  ArithmeticArray<Arithmetic, kN> result;
  for (uint row = 0; row < kN; ++row) {
    Arithmetic value = cast<Arithmetic>(0);
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
                                const Point<Arithmetic, kN>& point) noexcept
{
  return Point<Arithmetic, kN>{matrix * point.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator*(const Matrix<Arithmetic, kN, kN>& matrix,
                                 const Vector<Arithmetic, kN>& vector) noexcept
{
  return Vector<Arithmetic, kN>{matrix * vector.data()};
}

} // namespace zisc

#endif // ZISC_MATRIX_INL_HPP
