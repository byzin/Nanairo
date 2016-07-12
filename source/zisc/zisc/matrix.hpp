/*!
  \file matrix.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_MATRIX_HPP
#define ZISC_MATRIX_HPP

// Zisc
#include "arithmetic_array.hpp"
#include "dimension.hpp"
#include "point.hpp"
#include "vector.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kRow, uint kColumn>
class Matrix
{
  static_assert(0 < kRow, "kRow size is zero.");
  static_assert(0 < kColumn, "kColumn size is zero.");

 public:
  using ArrayType = ArithmeticArray<Arithmetic, kRow * kColumn>;
  using ValueType = Arithmetic;


  //! Initialize with 0
  Matrix() noexcept;

  //! Initialize with values
  template <typename ...Types>
  Matrix(const Types ...elements) noexcept;


  //! Check if two matrixes are same
  bool operator==(const Matrix& matrix) const noexcept;

  //! Check if two matrixes are not same
  bool operator!=(const Matrix& matrix) const noexcept;

  //! Apply multiplication operation
  Matrix operator*(const Arithmetic scalar) const noexcept;

  //! Return the reference by row and column
  Arithmetic& operator()(const uint row, const uint column) noexcept;

  //! Return the reference by row and column
  const Arithmetic& operator()(const uint row, const uint column) const noexcept;


  //! Calculate the cofactor
  Arithmetic cofactor(const uint row, const uint column) const noexcept;

  //! Return the cofactor matrix
  Matrix cofactorMatrix() const noexcept;

  //! Return column size
  static constexpr uint columnSize() noexcept;

  //! Calculate the determinant
  Arithmetic determinant() const noexcept;

  //! Return the reference by row and column
  Arithmetic& get(const uint row, const uint column) noexcept;

  //! Return the reference by row and column
  const Arithmetic& get(const uint row, const uint column) const noexcept;

  //! Return the inverse matrix
  Matrix inverseMatrix() const noexcept;

  //! Check if the matrix is square matrix
  static constexpr bool isSquareMatrix() noexcept;

  //! Calculate the minor determinant
  Arithmetic minorDeterminant(const uint row, const uint column) const noexcept;

  //! Return row size
  static constexpr uint rowSize() noexcept;

  //! Set element
  void set(const uint row, const uint column, const Arithmetic value) noexcept;

  //! Return the transposed matrix
  Matrix<Arithmetic, kColumn, kRow> transposedMatrix() const noexcept;

 private:
  ArrayType elements_;
};

//! Multiply two matrixes
template <typename Arithmetic, uint L, uint M, uint N>
Matrix<Arithmetic, L, N> operator*(const Matrix<Arithmetic, L, M>& a,
                                   const Matrix<Arithmetic, M, N>& b) noexcept;

//! Multiply two matrixes
template <typename Arithmetic, uint kRow, uint kColumn>
Matrix<Arithmetic, kRow, kColumn> operator*(
    const Arithmetic scalar,
    const Matrix<Arithmetic, kRow, kColumn>& matrix) noexcept;

//! Multiply a matrix and a array
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> operator*(
    const Matrix<Arithmetic, kN, kN>& matrix,
    const ArithmeticArray<Arithmetic, kN>& array) noexcept;

//! Multiply a matrix and a point
template <typename Arithmetic, uint kN>
Point<Arithmetic, kN> operator*(const Matrix<Arithmetic, kN, kN>& matrix,
                                const Point<Arithmetic, kN>& point) noexcept;

//! Multiply a matrix and a point
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator*(const Matrix<Arithmetic, kN, kN>& matrix,
                                 const Vector<Arithmetic, kN>& vector) noexcept;

} // namespace zisc

#include "matrix-inl.hpp"

#endif // ZISC_MATRIX_HPP
