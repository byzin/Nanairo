/*!
  \file math.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_MATH_HPP
#define ZISC_MATH_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <tuple>
// Zisc
#include "zisc/zisc_config.hpp"

namespace zisc {

//! Napier's constant
template <typename Arithmetic>
constexpr Arithmetic kE = 
    static_cast<Arithmetic>(2.718281828459045235360287471352662497e+00l);
//! Euler constant
template <typename Arithmetic>
constexpr Arithmetic kEuler = 
    static_cast<Arithmetic>(5.772156649015328606065120900824024310e-01l);

//! Calculate the circular constant
template <typename Float>
constexpr Float calculatePi() noexcept;

//! Circular constant
template <typename Float>
constexpr Float kPi = calculatePi<Float>();

// Basic operations

//! Return the absolute value
template <typename Arithmetic>
constexpr Arithmetic abs(const Arithmetic n) noexcept;

//! Return the greatest common divisor of two integer m and n
template <typename Integral>
constexpr Integral gcd(Integral m, Integral n) noexcept;

//!
template <typename Integer>
constexpr Integer factorial(const Integer n) noexcept;

//! Return a inverse number
template <typename Float>
constexpr Float invert(const Float x) noexcept;

//!
template <typename Integer>
constexpr Integer sequence(const Integer m, const Integer n) noexcept;

// Power functions

//! Calculate x^(exponent)
template <uint kExponent, typename Arithmetic>
constexpr Arithmetic power(const Arithmetic x) noexcept;

//! Calculate x^(exponent)
template <typename Arithmetic>
Arithmetic power(Arithmetic x, int exponent) noexcept;

//! pow
template <typename Float>
Float pow(const Float base, const Float exponent) noexcept;

//! Calculate the inverse square root
template <typename Float>
Float invSqrt(const Float n) noexcept;

//! Calculate the square root
template <typename Float>
Float sqrt(const Float n) noexcept;

//! Calculate the cubic root
template <typename Float>
Float cbrt(const Float n) noexcept;

// Exponential functions

//! Calculate the exponential
template <typename Float>
Float exp(const Float n) noexcept;

//! eを底とするlogを求めます
template <typename Float>
Float log(const Float x) noexcept;

//! 2を底とするlogを求めます
float log2(const float x) noexcept;

// Trigonometric functions

//! Calculate sin(theta)
template <typename Float>
Float sin(const Float theta) noexcept;

//! Calculate cos(theta)
template <typename Float>
Float cos(const Float theta) noexcept;

//! Calculate tan(theta)
template <typename Float>
Float tan(const Float theta) noexcept;

//! Calculate asin(x)
template <typename Float>
Float asin(const Float x) noexcept;

//! Calculate acos(x)
template <typename Float>
Float acos(const Float x) noexcept;

//! Calculate atan(x)
template <typename Float>
Float atan(const Float x) noexcept;

// Polynomial

//! Solve a quadratic equation
template <typename Float>
std::tuple<std::array<Float, 2>, uint> solveQuadratic(const Float a, 
                                                      const Float b, 
                                                      const Float c) noexcept;

//! Solve a cubic equation
template <typename Float>
Float solveCubicOne(const Float b, 
                    const Float c, 
                    const Float d) noexcept;

//! Solve a cubic equation
template <typename Float>
Float solveCubicOne(const Float a, 
                    const Float b, 
                    const Float c, 
                    const Float d) noexcept;

//! Solve a quartic equation
template <typename Float>
std::tuple<std::array<Float, 4>, uint> solveQuartic(const Float a, 
                                                    const Float b, 
                                                    const Float c, 
                                                    const Float d, 
                                                    const Float e) noexcept;

} // namespace zisc

#include "math-inl.hpp"

#endif // ZISC_MATH_HPP
