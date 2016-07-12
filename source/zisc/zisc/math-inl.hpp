/*!
  \file math-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_MATH_INL_HPP
#define ZISC_MATH_INL_HPP

#include "math.hpp"
// Standard C++ library
#include <cmath>
#include <limits>
#include <type_traits>
// Zisc
#include "error.hpp"
#include "fraction.hpp"
#include "floating_point.hpp"
#include "utility.hpp"
#include "type_traits.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

#if defined(ZISC_MATH_EFFICIENT_POWER)
//#define ZISC_MATH_POW
//#define ZISC_MATH_INVERSE_SQRT
#define ZISC_MATH_SQRT
#endif // ZISC_MATH_EFFICIENT_POWER

#if defined(ZISC_MATH_EFFICIENT_TRIGONOMETRIC)
#define ZISC_MATH_SIN
#define ZISC_MATH_COS
#define ZISC_MATH_TAN
#define ZISC_MATH_ASIN
#define ZISC_MATH_ACOS
#define ZISC_MATH_ATAN
#endif // ZISC_MATH_EFFICIENT_TRIGONOMETRIC

/*!
  */
template <typename Float> inline
constexpr Float calculatePi() noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point.");
  constexpr int64 n = cast<int64>(sizeof(Float)) * 10ll;
  constexpr Float t = cast<Float>(2.0);
  Float pi = t;
  for (int64 k = n; 0ll < k; --k) {
    const Float a = Fraction64{k, 2ll * k + 1ll}.toFloat<Float>();
    pi = t + a * pi;
  }
  return pi;
}

/*!
 \details
 No detailed.
 */
template <typename Signed> inline
constexpr Signed abs(const Signed n) noexcept
{
  static_assert(std::is_signed<Signed>::value, "Signed isn't signed type");
  return isNegative(n) ? -n : n;
}

/*!
 \details
 No detailed.
 */
template <typename Integer> inline
constexpr Integer gcd(Integer m, Integer n) noexcept
{
  static_assert(kIsInteger<Integer>, "The m and n aren't integer type.");
  while (n != 0) {
    const auto tmp = n;
    n = (m < n) ? m : m % n;
    m = tmp;
  }
  return m;
}

/*!
  */
template <typename Integer> inline
constexpr Integer factorial(const Integer n) noexcept
{
  static_assert(kIsInteger<Integer>, "Integer isn't integer type.");
  Integer x = 1;
  for (Integer i = 1; i < n; ++i)
    x = x * (i + 1);
  return x;
}

/*!
  */
template <typename Float> inline
constexpr Float invert(const Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point.");
  constexpr Float one = cast<Float>(1.0);
  return one / x;
}

/*!
  */
template <typename Integer> inline
constexpr Integer sequence(const Integer m, const Integer n) noexcept
{
  static_assert(kIsInteger<Integer>, "Integer isn't integer type.");
  Integer x = 1;
  for (Integer i = m; i < n; ++i)
    x = x * (i + 1);
  return x;
}

/*!
  \details
  No detailed.
  */
template <uint kExponent, typename Arithmetic> inline
constexpr Arithmetic power(const Arithmetic x) noexcept
{
  static_assert(std::is_arithmetic<Arithmetic>::value, 
                "Arithmetic isn't arithmetic type");
  constexpr uint half_e = kExponent >> 1;
  constexpr bool exponent_is_odd = isOdd(kExponent);
  return (kExponent == 0) ? cast<Arithmetic>(1) :
         (kExponent == 1) ? x :
         (exponent_is_odd) ? (x * power<half_e>(x) * power<half_e>(x))
                           : (power<half_e>(x) * power<half_e>(x));
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic>
Arithmetic power(Arithmetic x, int exponent) noexcept
{
  static_assert(std::is_arithmetic<Arithmetic>::value, 
                "Arithmetic isn't arithmetic type");
  if (isNegative(exponent)) {
    x = cast<Arithmetic>(1) / x;
    exponent = -exponent;
  }
  Arithmetic result = cast<Arithmetic>(1);
  while (exponent) {
    if (isOdd(exponent))
      result = x * result;
    x = power<2>(x);
    exponent = exponent >> 1;
  }
  return result;
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float pow(const Float base, const Float exponent) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
#ifndef ZISC_MATH_POW
  return std::pow(base, exponent);
#else // ZISC_MATH_POW
  static_assert(false, "Optimized pow() is not implemented.");
#endif // ZISC_MATH_POW
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float invSqrt(const Float n) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
#ifndef ZISC_MATH_INVERSE_SQRT
  return cast<Float>(1.0) / sqrt(n);
#else // ZISC_MATH_INVERSE_SQRT
  static_assert(false, "Optimized invSqrt() is not implemented.");
#endif // ZISC_MATH_INVERSE_SQRT
}

namespace zisc_math {

template <int64 n, typename Float>
struct TaylorCoefficient
{
  static_assert(0 < n, "The n isn't positive.");
  static_assert(kIsFloat<Float>, "Float isn't floating point.");

  static constexpr Float arctan() noexcept
  {
    const auto k = Fraction64{power<n>(-1ll), (2ll * n + 1ll)};
    return k.toFloat<Float>();
  }

  static constexpr Float sin() noexcept
  {
    const Fraction64 k{power<n>(-1ll), factorial(2ll * n + 1ll)};
    return k.toFloat<Float>();
  }
  
  static constexpr Float sqrt() noexcept
  {
    int64 p = 1;
    for (int64 i = 1; i < n; ++i)
      p = p * (1ll - 2ll * i);
    const auto k = Fraction64{p, power<n>(2ll)} * Fraction64{1, factorial(n)};
    return k.toFloat<Float>();
  }
};

template <int64 n, int64 end, bool is_last = (n == end)>
struct IterationMethod
{
  static_assert(0 < n, "The n isn't positive.");
  static_assert(0 < end, "The end isn't positive.");

  template <typename Float>
  static constexpr Float sinTaylor(const Float x) noexcept
  {
    static_assert(kIsFloat<Float>, "Float isn't floating point.");
    constexpr auto k = TaylorCoefficient<n, Float>::sin();
    return power<2>(x) * (k + IterationMethod<n + 1, end>::sinTaylor(x));
  }

  template <typename Float>
  static constexpr Float arctanTaylor(const Float x) noexcept
  {
    static_assert(kIsFloat<Float>, "Float isn't floating point.");
    constexpr auto k = TaylorCoefficient<n, Float>::arctan();
    return power<2>(x) * (k + IterationMethod<n + 1, end>::arctanTaylor(x));
  }

  template <typename Float>
  static constexpr Float sqrtTaylor(const Float x) noexcept
  {
    static_assert(kIsFloat<Float>, "Float isn't floating point.");
    constexpr Float k = TaylorCoefficient<n, Float>::sqrt();
    return x * (k + IterationMethod<n + 1, end>::sqrtTaylor(x));
  }

  template <typename Float>
  static constexpr Float sqrtNewton(const Float s, const Float x) noexcept
  {
    static_assert(kIsFloat<Float>, "Float isn't floating point.");
    constexpr Float half = cast<Float>(0.5);
    const Float xn = IterationMethod<n + 1, end>::sqrtNewton(s, x);
    return half * (xn + s / xn);
  }
};

template <int64 n, int64 end>
struct IterationMethod<n, end, true>
{
  static_assert(0 < n, "The n isn't positive.");
  static_assert(0 < end, "The end isn't positive.");

  template <typename Float>
  static constexpr Float sinTaylor(const Float x) noexcept
  {
    static_assert(kIsFloat<Float>, "Float isn't floating point.");
    constexpr auto k = TaylorCoefficient<n, Float>::sin();
    return power<2>(x) * k;
  }

  template <typename Float>
  static constexpr Float arctanTaylor(const Float x) noexcept
  {
    static_assert(kIsFloat<Float>, "Float isn't floating point.");
    constexpr auto k = TaylorCoefficient<n, Float>::arctan();
    return power<2>(x) * k;
  }

  template <typename Float>
  static constexpr Float sqrtTaylor(const Float x) noexcept
  {
    static_assert(kIsFloat<Float>, "Float isn't floating point.");
    return x * TaylorCoefficient<n, Float>::sqrt();
  }

  template <typename Float>
  static constexpr Float sqrtNewton(const Float s, const Float x) noexcept
  {
    static_assert(kIsFloat<Float>, "Float isn't floating point.");
    constexpr Float half = cast<Float>(0.5);
    return half * (x + s / x);
  }
};

/*!
  */
template <int64 max_n = 4, typename Float> inline
constexpr Float sqrtTaylor(const Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  static_assert(0 < max_n, "The n isn't positive.");
  constexpr Float one = cast<Float>(1.0);
  return one + IterationMethod<1, max_n>::sqrtTaylor(x - one);
}

/*!
  */
template <int64 max_n = 4, typename Float> inline
constexpr Float sqrtNewton(const Float s) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  static_assert(0 < max_n, "The n isn't positive.");
  const Float x0 = sqrtTaylor<8>(s);
  return IterationMethod<1, max_n>::sqrtNewton(s, x0);
}

/*!
  */
template <int64 max_n = 3, typename Float> inline
constexpr Float sqrt(const Float x) noexcept
{
  // Exponent sqrt
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  constexpr int N = 8;
  constexpr auto root2 = sqrtNewton<N>(cast<Float>(2.0));
  constexpr auto inverse_root2 = sqrtNewton<N>(cast<Float>(0.5));
  static_assert(root2 == sqrtNewton<N+1>(cast<Float>(2.0)),
                "sqrt(2.0) isn't converged.");
  static_assert(inverse_root2 == sqrtNewton<N+1>(cast<Float>(0.5)),
                "sqrt(0.5) isn't converged.");
  using FBit = FloatingPointBit<Float>;
  const auto exponent = FBit::halfExponentBits(FBit::getExponentBits(x));
  const Float e = FBit::makeFloat(cast<typename FBit::BitType>(0), exponent);
  const Float s = (!FBit::isOddExponent(x))
      ? cast<Float>(1.0) : (FBit::isPositiveExponent(x))
      ? root2
      : inverse_root2;
  // Mantissa sqrt
  auto m = FBit::makeFloat(FBit::getMantissaBits(x), FBit::exponentValueBitMask());
  m = sqrtNewton<max_n>(m);
  return m * s * e;
}

} // namespace zisc_math

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float sqrt(const Float n) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
#ifndef ZISC_MATH_SQRT
  return std::sqrt(n);
#else // ZISC_MATH_SQRT 
  return zisc_math::sqrt(n);
#endif // ZISC_MATH_SQRT 
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float cbrt(const Float n) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
#ifndef ZISC_MATH_CBRT
  return std::cbrt(n);
#else // ZISC_MATH_CBRT
  static_assert(false, "Optimized cbrt() is not implemented.");
#endif // ZISC_MATH_CBRT
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float exp(const Float n) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
#ifndef ZISC_MATH_EFFICIENT_EXPONENTIAL
  return std::exp(n);
#else // ZISC_MATH_EFFICIENT_EXPONENTIAL
  static_assert(false, "Optimized exp() is not implemented.");
#endif // ZISC_MATH_EFFICIENT_EXPONENTIAL
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float log(const Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
#ifndef ZISC_MATH_EFFICIENT_EXPONENTIAL
  return std::log(x);
#else // ZISC_MATH_EFFICIENT_EXPONENTIAL
  static_assert(false, "Optimized log() is not implemented.");
#endif // ZISC_MATH_EFFICIENT_EXPONENTIAL
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float log2(const Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
#ifndef ZISC_MATH_EFFICIENT_EXPONENTIAL
  return std::log2(x);
#else // ZISC_MATH_EFFICIENT_EXPONENTIAL
  static_assert(false, "Optimized log2() is not implemented.");
#endif // ZISC_MATH_EFFICIENT_EXPONENTIAL
}

namespace zisc_math {

/*!
  */
template <typename Float> inline
constexpr Float sin(Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  return x + (x * IterationMethod<1, 9>::sinTaylor(x));
}

/*!
  */
template <typename Float> inline
constexpr Float cos(const Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  constexpr Float half_pi = cast<Float>(0.5) * kPi<Float>;
  return sin(half_pi - x);
}

/*!
  */
template <typename Float> inline
constexpr Float tan(Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  return sin(x) / cos(x);
}

/*!
  */
template <typename Float> inline
Float arctan(Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  constexpr Float half_pi = cast<Float>(0.5) * kPi<Float>;
  ZISC_ASSERT(isInClosedBounds(x, -half_pi, half_pi), 
              "The x is out of range [-pi, pi].");
  constexpr Float half = cast<Float>(0.5);
  constexpr Float one = cast<Float>(1.0);
  constexpr Float twice = cast<Float>(2.0);

  // Check if x is negative
  const bool is_negative_number = isNegative(x);
  x = abs(x);
  // Check if x is greater than one
  const bool is_greater_than_one = (one < x);
  x = (is_greater_than_one) ? invert(x) : x;
  // Check if x is greater than half
  const bool is_greater_than_half = (half < x);
  x = (is_greater_than_half) ?  (-one + zisc::sqrt(one + power<2>(x))) / x : x;
  // Calculate arctan(x)
  auto result = x + (x * IterationMethod<1, 24>::arctanTaylor(x));
  //
  result = (is_greater_than_half) ? twice * result : result;
  result = (is_greater_than_one) ? half_pi - result : result;
  return (is_negative_number) ? -result : result;
}

/*!
  */
template <typename Float> inline
Float arcsin(const Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  constexpr Float one = cast<Float>(1.0);
  constexpr Float twice = cast<Float>(2.0);
  ZISC_ASSERT(isInClosedBounds(x, -one, one), "The x is out of range [-1, 1].");
  return twice * arctan(x / (one + sqrt(one - power<2>(x))));
}

/*!
  */
template <typename Float> inline
Float arccos(const Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  constexpr Float one = cast<Float>(1.0);
  constexpr Float twice = cast<Float>(2.0);
  ZISC_ASSERT(isInClosedBounds(x, -one, one), "The x is out of range [-1, 1].");
  return twice * arctan(sqrt(one - power<2>(x)) / (one + x));
}

} // namespace zisc_math

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float sin(const Float theta) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  ZISC_ASSERTION_STATEMENT(constexpr auto half_pi = cast<Float>(0.5) * kPi<Float>);
  ZISC_ASSERT(isInClosedBounds(theta, -half_pi, half_pi),
              "The theta is out of range.");
#ifndef ZISC_MATH_SIN
  return std::sin(theta);
#else // ZISC_MATH_SIN
  return zisc_math::sin(theta);
#endif // ZISC_MATH_SIN
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float cos(const Float theta) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  ZISC_ASSERTION_STATEMENT(constexpr auto pi = kPi<Float>);
  ZISC_ASSERT(isInClosedBounds(theta, 0.0, pi), 
              "The theta is out of range.");
#ifndef ZISC_MATH_COS
  return std::cos(theta);
#else // ZISC_MATH_COS
  return zisc_math::cos(theta);
#endif // ZISC_MATH_COS
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float tan(const Float theta) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  ZISC_ASSERT(theta != cast<Float>(0.0), "The x is 0.");
  ZISC_ASSERTION_STATEMENT(constexpr auto half_pi = cast<Float>(0.5) * kPi<Float>);
  ZISC_ASSERT(isInClosedBounds(theta, cast<Float>(0.0), half_pi),
              "The theta is out of range.");
#ifndef ZISC_MATH_TAN
  return std::tan(theta);
#else // ZISC_MATH_TAN
  return zisc_math::tan(theta);
#endif // ZISC_MATH_TAN
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float asin(const Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  ZISC_ASSERTION_STATEMENT(constexpr auto one = cast<Float>(1.0));
  ZISC_ASSERT(isInClosedBounds(x, -one, one),
              "The x is out of range.");
#ifndef ZISC_MATH_ASIN
  return std::asin(x);
#else // ZISC_MATH_ASIN
  return zisc_math::arcsin(x);
#endif // ZISC_MATH_ASIN
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float acos(const Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  ZISC_ASSERTION_STATEMENT(constexpr auto one = cast<Float>(1.0));
  ZISC_ASSERT(isInClosedBounds(x, -one, one),
              "The x is out of range.");
#ifndef ZISC_MATH_ACOS
  return std::acos(x);
#else // ZISC_MATH_ACOS
  return zisc_math::arccos(x);
#endif // ZISC_MATH_ACOS
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float atan(const Float x) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");
  ZISC_ASSERT(x != cast<Float>(0.0), "The x is 0.");
#ifndef ZISC_MATH_ATAN
  return std::atan(x);
#else // ZISC_MATH_ATAN
  return zisc_math::arctan(x);
#endif // ZISC_MATH_ATAN
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
std::tuple<std::array<Float, 2>, uint> solveQuadratic(const Float a, 
                                                      const Float b, 
                                                      const Float c) noexcept
{
  constexpr Float min_x = std::numeric_limits<Float>::lowest();
  std::array<Float, 2> x = {{min_x, min_x}};
  uint n = 0;

  const Float discriminant = b * b - 4.0 * a * c;
  if (discriminant == 0.0) {
    x[0] = -b / (2.0 * a);
    n = 1;
  }
  else if (0.0 < discriminant) {
    const Float root_d = sqrt(discriminant);
    x[0] = (-b + root_d) / (2.0 * a);
    x[1] = (-b - root_d) / (2.0 * a);
    n = 2;
  }
  return std::make_tuple(x, n);
}

namespace zisc_math {

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float solveCubicOneY(const Float p, const Float q) noexcept
{
//  ZISC_ASSERT(p != 0.0, "The p is zero.");
//  ZISC_ASSERT(q != 0.0, "The q is zero.");
  if (p == 0.0)
    return cbrt(-q);
  else if (q == 0.0)
    return sqrt(-p);

  Float y;
  const Float discriminant = q * q + (4.0 / 27.0) * p * p * p;
  if (0.0 <= discriminant) {
    const Float z = cbrt((1.0 / 2.0) * (sqrt(discriminant) - q));
    y = z - (p / (3.0 * z));
  }
  else {
    ZISC_ASSERT(p < 0.0, "The p is positive: ", p);
    const Float r = sqrt((-1.0 / 3.0) * p);
    const Float cos_theta = q / (-2.0 * r * r * r);
    const Float phi = (1.0 / 3.0) * acos(cos_theta);
    y = 2.0 * r * cos(phi);
  }
  return y;
}

} // namespace zisc_math

/*!
  \details
  Solve a cubic equation using "cubic reduction" method.
  */
template <typename Float> inline
Float solveCubicOne(const Float b, const Float c, const Float d) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");

  const Float k = (-1.0 / 3.0) * b;

  const Float p = (b * k + c);
  const Float q = (((2.0 / 3.0) * b * k + c) * k + d);
  const Float y = zisc_math::solveCubicOneY(p, q);
  const Float x = y  +  k;
  return x;
}

/*!
  \details
  Solve a cubic equation using "cubic reduction" method.
  */
template <typename Float> inline
Float solveCubicOne(const Float a, 
                    const Float b, 
                    const Float c, 
                    const Float d) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");

  ZISC_ASSERT(a != 0.0, "The a is zero.");
  const Float inv_a = 1.0 / a;
  const Float k = (-1.0 / 3.0) * b * inv_a;

  const Float p = (b * k + c) * inv_a;
  const Float q = (((2.0 / 3.0) * b * k + c) * k + d) * inv_a;
  const Float y = zisc_math::solveCubicOneY(p, q);
  const Float x = y  +  k;
  return x;
}

namespace zisc_math {

/*!
  \details
  No detailed.
  */
template <typename Float> inline
void sortQuarticResults(const std::array<Float, 2>& x1,
                        const std::array<Float, 2>& x2,
                        std::array<Float, 4>& x) noexcept
{
  x[0] = max(x1[0], x2[0]);
  const Float tmp1 = min(x1[0], x2[0]);
  const Float tmp2 = max(x1[1], x2[1]);
  x[1] = max(tmp1, tmp2);
  x[2] = min(tmp1, tmp2);
  x[3] = min(x1[1], x2[1]);
}

} // namespace zisc_math

/*!
  \details
  No detailed.
  */
template <typename Float> inline
std::tuple<std::array<Float, 4>, uint> solveQuartic(const Float a, 
                                                    const Float b, 
                                                    const Float c, 
                                                    const Float d, 
                                                    const Float e) noexcept
{
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");

  ZISC_ASSERT(a != 0.0, "The a is zero.");
  const Float inv_a = 1.0 / a;
  const Float k = (-1.0 / 4.0) * b * inv_a;

  const Float p = ((3.0 / 2.0) * b * k + c) * inv_a;
  const Float q = ((b * k + c) * 2.0 * k + d) * inv_a;
  const Float r = ((((3.0 / 4.0) * b * k + c) * k + d) * k + e) * inv_a;
  const Float z = solveCubicOne(-0.5 * p, -r, (1.0 / 8.0) * (4 * p * r - q * q));

  uint n = 0;
  std::array<Float, 4> x;

  const Float f2 = 2.0 * z - p;
  if (0.0 <= f2) {
    constexpr Float min_x = std::numeric_limits<Float>::lowest();
    const Float f = sqrt(f2);
    std::array<Float, 2> x1 = {{min_x, min_x}};
    const Float d1 = -(2.0 * (z + (q / f)) + p);
    if (0.0 <= d1) {
      const Float root_d1 = sqrt(d1);
      x1[0] = 0.5 * (f + root_d1) + k;
      x1[1] = 0.5 * (f - root_d1) + k;
      n += 2;
    }
    std::array<Float, 2> x2 = {{min_x, min_x}};
    const Float d2 = -(2.0 * (z - (q / f)) + p);
    if (0.0 <= d2) {
      const Float root_d2 = sqrt(d2);
      x2[0] = 0.5 * (-f + root_d2) + k;
      x2[1] = 0.5 * (-f - root_d2) + k;
      n += 2;
    }
    zisc_math::sortQuarticResults(x1, x2, x);
  }
  return std::make_tuple(x, n);
}

} // namespace zisc

#endif // ZISC_MATH_INL_HPP
