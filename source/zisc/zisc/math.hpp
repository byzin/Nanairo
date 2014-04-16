/*!
  \file math.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_MATH_HPP_
#define _ZISC_MATH_HPP_

// Standard C++ library
#include <array>
#include <cstddef>
#include <tuple>
// Zisc
#include "zisc/zisc_config.hpp"

namespace zisc {

constexpr double kE = 2.718281828459045235360287471352; //!< Napier's constant
constexpr double kPi = 3.14159265358979323846264338327950288; //!< Circular constant

// Basic operations

//! 値の負が確認します
template <typename Signed>
constexpr bool isNegative(const Signed n);

//! Return the absolute value
template <typename Arithmetic>
constexpr Arithmetic abs(const Arithmetic n);

//! 最大公約数を取得します
template <typename Integral>
constexpr Integral gcd(Integral m, Integral n);

// Power functions

//! Calculate x^(exponent)
template <uint kExponent, typename Arithmetic>
constexpr Arithmetic power(const Arithmetic x);

//! Calculate x^(exponent)
template <typename Arithmetic>
Arithmetic power(Arithmetic x, int exponent);

//! pow
template <typename Float>
Float pow(const Float base, const Float exp);

//! Calculate the inverse square root
template <typename Float>
Float invSqrt(const Float n);

//! Calculate the square root
template <typename Float>
Float sqrt(const Float n);

//! Calculate the cubic root
template <typename Float>
Float cbrt(const Float n);

// Exponential functions

//! Calculate the exponential
template <typename Float>
Float exp(const Float n);

//! eを底とするlogを求めます
template <typename Float>
Float log(const Float x);

//! 2を底とするlogを求めます
float log2(const float x);

// Trigonometric functions

//! Calculate sin(theta)
template <typename Float>
Float sin(const Float theta);

//! Calculate cos(theta)
template <typename Float>
Float cos(const Float theta);

//! Calculate arcsin(x)
template <typename Float>
Float arcSin(const Float x);

//! Calculate arccos(x)
template <typename Float>
Float arcCos(const Float x);

//! Calculate arctan(x)
template <typename Float>
Float arcTan(const Float x);

// Polynomial

//! Solve a quadratic equation
template <typename Float>
std::tuple<std::array<Float, 2>, uint> solveQuadratic(
      const Float a, const Float b, const Float c);

//! Solve a cubic equation
template <typename Float>
Float solveCubicOne(const Float b, const Float c, const Float d);

//! Solve a cubic equation
template <typename Float>
Float solveCubicOne(const Float a, const Float b, const Float c, const Float d);

//! Solve a quartic equation
template <typename Float>
std::tuple<std::array<Float, 4>, uint> solveQuartic(
      const Float a, const Float b, const Float c, const Float d, const Float e);

} // namespace zisc

#include "math-inl.hpp"

#endif // _ZISC_MATH_HPP_
