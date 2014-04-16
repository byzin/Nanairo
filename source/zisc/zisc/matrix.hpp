/*!
  \file matrix.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_MATRIX_HPP_
#define _ZISC_MATRIX_HPP_

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
  static_assert(kRow > 0, "## kRow size must be greater than 0.");
  static_assert(kColumn > 0, "## kColumn size must be greater than 0.");

 public:
  using ArrayType = ArithmeticArray<Arithmetic, kRow * kColumn>;
  using ValueType = Arithmetic;


  //! Initialize with 0
  Matrix();

  //! Initialize with values
  template <typename ...Types>
  Matrix(const Types ...elements);


  //! Check if two matrixes are same
  bool operator==(const Matrix& matrix) const;

  //! Check if two matrixes are not same
  bool operator!=(const Matrix& matrix) const;

  //! Apply multiplication operation
  Matrix operator*(const Arithmetic scalar) const;

  //! Return the element by row and column
  Arithmetic& operator()(const uint row, const uint column);

  //! Return the element by row and column
  const Arithmetic& operator()(const uint row, const uint column) const;


  //! Calculate the cofactor
  Arithmetic cofactor(const uint row, const uint column) const;

  //! Return the cofactor matrix
  Matrix cofactorMatrix() const;

  //! Return column size
  static constexpr uint columnSize();

  //! Calculate the determinant
  Arithmetic determinant() const;

  //! Return the element by row and column
  Arithmetic get(const uint row, const uint column) const;

  //! Return the inverse matrix
  Matrix inverseMatrix() const;

  //! Check if the matrix is square matrix
  static constexpr bool isSquareMatrix();

  //! Calculate the minor determinant
  Arithmetic minorDeterminant(const uint row, const uint column) const;

  //! Return row size
  static constexpr uint rowSize();

  //! Set element
  void set(const uint row, const uint column, const Arithmetic value);

  //! Return the transposed matrix
  Matrix<Arithmetic, kColumn, kRow> transposedMatrix() const;

 private:
  ArrayType elements_;
};

//! Multiply two matrixes
template <typename Arithmetic, uint L, uint M, uint N>
Matrix<Arithmetic, L, N> operator*(const Matrix<Arithmetic, L, M>& a,
                                   const Matrix<Arithmetic, M, N>& b);

//! Multiply two matrixes
template <typename Arithmetic, uint kRow, uint kColumn>
Matrix<Arithmetic, kRow, kColumn> operator*(
    const Arithmetic scalar,
    const Matrix<Arithmetic, kRow, kColumn>& matrix);

//! Multiply a matrix and a array
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> operator*(
    const Matrix<Arithmetic, kN, kN>& matrix,
    const ArithmeticArray<Arithmetic, kN>& array);

//! Multiply a matrix and a point
template <typename Arithmetic, uint kN>
Point<Arithmetic, kN> operator*(const Matrix<Arithmetic, kN, kN>& matrix,
                                const Point<Arithmetic, kN>& point);

//! Multiply a matrix and a point
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator*(const Matrix<Arithmetic, kN, kN>& matrix,
                                 const Vector<Arithmetic, kN>& vector);

} // namespace zisc

#include "matrix-inl.hpp"

#endif // _ZISC_MATRIX_HPP_
