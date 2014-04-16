/*!
  \file math-inl.h
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_MATH_INL_HPP_
#define _ZISC_MATH_INL_HPP_

#include "math.hpp"
// Standard C++ library
#include <cmath>
#include <limits>
#include <type_traits>
// Zisc
#include "error.hpp"
#include "utility.hpp"
#include "type_traits.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
 \details
 No detailed.

 \param[in] n 算術値
 \returh nが負ならばtrue、そうでなければfalse
 */
template <typename Signed> inline
constexpr bool isNegative(const Signed n)
{
  static_assert(std::is_signed<Signed>::value, "n must be signed type.");
  return n < cast<Signed>(0);
}

/*!
 \details
 No detailed.

 \param[in] n 符号付きの値
 \return nの絶対値
 */
template <typename Signed> inline
constexpr Signed abs(const Signed n)
{
  static_assert(std::is_signed<Signed>::value, "n must be signed type");
  return isNegative(n) ? -n : n;
}

/*!
 \details
 ユークリッドの互除法を用います

 \param[in] m 自然数
 \param[in] n 自然数
 \return mとnの最大公約数
 */
template <typename Integer> inline
constexpr Integer gcd(Integer m, Integer n)
{
  static_assert(std::is_integral<Integer>::value, "m, n must be integer type.");
  while (n != 0) {
    const auto tmp = n;
    n = (m < n) ? m : m % n;
    m = tmp;
  }
  return m;
}

/*!
  \details
  No detailed.
  */
template <uint kExponent, typename Arithmetic> inline
constexpr Arithmetic power(const Arithmetic x)
{
  static_assert(std::is_arithmetic<Arithmetic>::value,"x must be arithmetic type");
  constexpr uint half_e = kExponent >> 1;;
  return (kExponent == 0)       ? cast<Arithmetic>(1) : 
         (kExponent == 1)       ? x : 
         ((kExponent & 1) == 1) ? x * power<half_e>(x) * power<half_e>(x)
                                : power<half_e>(x) * power<half_e>(x);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic>
Arithmetic power(Arithmetic x, int exponent)
{
  static_assert(std::is_arithmetic<Arithmetic>::value,"x must be arithmetic type");
  if (isNegative(exponent)) {
    x = cast<Arithmetic>(1) / x;
    exponent = -exponent;
  }
  Arithmetic result = cast<Arithmetic>(1);
  while (exponent) {
    if ((exponent & 1) == 1)
      result = result * x;
    x = x * x;
    exponent = exponent >> 1;
  }
  return result;
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float pow(const Float base, const Float exp)
{
  static_assert(kIsFloat<Float>, "Arguments must be floating point.");
#ifndef ZISC_MATH_FAST_POW
  return std::pow(base, exp);
#else // ZISC_MATH_FAST_POW
  static_assert(false, "Approximate pow() is not implemented.");
#endif // ZISC_MATH_FAST_POW
}

namespace zisc_math {

inline
float approxInvSqrt(const float n)
{
  constexpr int32 kMagicNumber = 0x5f3759df;
  union {float f; int32 i;} u{n};
  u.i = kMagicNumber - (u.i >> 1);
  u.f = u.f * (1.5f - 0.5f * n * u.f * u.f);
  return u.f * (1.5f - 0.5f * n * u.f * u.f);
}

inline
double approxInvSqrt(const double n)
{
  constexpr int64 kMagicNumber = 0x5fe6eb50c7b537aaLL;
  union {double f; int64 i;} u{n};
  u.i = kMagicNumber - (u.i >> 1);
  u.f = u.f * (1.5 - 0.5 * n * u.f * u.f);
  return u.f * (1.5 - 0.5 * n * u.f * u.f);
}

} // namespace zisc_math

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float invSqrt(const Float n)
{
  static_assert(kIsFloat<Float>, "Arguments must be floating point.");
#ifndef ZISC_MATH_FAST_SQRT
  return cast<Float>(1.0) / sqrt(n);
#else // ZISC_MATH_FAST_SQRT
  return zisc_math::approxInvSqrt(n);
#endif // ZISC_MATH_FAST_SQRT
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float sqrt(const Float n)
{
  static_assert(kIsFloat<Float>, "Arguments must be floating point.");
#ifndef ZISC_MATH_FAST_SQRT
  return std::sqrt(n);
#else // ZISC_MATH_FAST_SQRT
  return n * invSqrt(n);
#endif // ZISC_MATH_FAST_SQRT
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float cbrt(const Float n)
{
  static_assert(kIsFloat<Float>, "Arguments must be floating point.");
#ifndef ZISC_MATH_FAST_CBRT
  return std::cbrt(n);
#else // ZISC_MATH_FAST_CBRT
  ZISC_ASSERT(false, "The fast cbrt function is not implemented.");
#endif // ZISC_MATH_FAST_CBRT
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float exp(const Float n)
{
  static_assert(kIsFloat<Float>, "Argument must be floating point.");
#ifndef ZISC_MATH_FAST_EXP
  return std::exp(n);
#else // ZISC_MATH_FAST_EXP
  static_assert(false, "Approximate exp() is not implemented.");
#endif // ZISC_MATH_FAST_EXP
}

namespace zisc_math {

/*!
  \details
  No detailed.

  \param[in] x 
  */
inline
float approxLog2(const float x)
{
  union {float f; uint32 i;} const vx{x};
  union {uint32 i; float f;} const mx{(vx.i & 0x007FFFFF) | 0x3f000000};
  const float y = cast<float>(vx.i) * 1.1920928955078125e-7f;
  return y - 124.22551499f - 1.498030302f * mx.f - 1.72587999f / (0.3520887068f+mx.f);
}

inline
float appoxLog(const float x)
{
  return 0.69314718f * approxLog2(x);
}

} // namespace zisc_math

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float log(const Float x)
{
  static_assert(kIsFloat<Float>, "Argument must be floating point.");
#ifndef ZISC_MATH_FAST_LOG
  return std::log(x);
#else // ZISC_MATH_FAST_LOG
  return zisc_math::approxLog(x);
#endif // ZISC_MATH_FAST_LOG
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float log2(const Float x)
{
  static_assert(kIsFloat<Float>, "Argument must be floating point.");
#ifndef ZISC_MATH_FAST_LOG
  return std::log2(x);
#else // ZISC_MATH_FAST_LOG
  return zisc_math::approxLog2(x);
#endif // ZISC_MATH_FAST_LOG
}

namespace zisc_math {

/*!
 \details
 アルゴリズムの詳細はhttp://lab.polygonal.de/?p=205を参照して下さい。

 \attention theta は -pi <= theta < pi でなければなりません

 \tparam Float 浮動小数点型
 \tparam high_precision trueの場合、高精度な近似値を求めます

 \param[in] theta ラジアン
 \return sin(theta)の値
 */
template <typename Float, bool high_precision> inline
constexpr Float approxSin(const Float theta)
{
  static_assert(std::is_floating_point<Float>::value, "theta must be float type.");
  return high_precision
    ? approxSin<Float, false>(theta) * (isNegative(approxSin<Float, false>(theta))
      ? cast<Float>(0.775) - cast<Float>(0.225) * approxSin<Float, false>(theta)
      : cast<Float>(0.775) + cast<Float>(0.225) * approxSin<Float, false>(theta))
    : theta * (isNegative(theta)
      ? cast<Float>(1.2732395447) + cast<Float>(0.4052847346) * theta
      : cast<Float>(1.2732395447) - cast<Float>(0.4052847346) * theta);
}

/*!
 \details
 アルゴリズムの詳細はhttp://lab.polygonal.de/?p=205を参照して下さい。

 \attention theta は -pi <= theta < pi でなければなりません

 \tparam Float 浮動小数点型
 \tparam high_precision trueの場合、高精度な近似値を求めます

 \param[in] theta ラジアン
 \return cos(theta)の値
 */
template <typename Float, bool high_precision> inline
constexpr Float approxCos(const Float theta)
{
  static_assert(std::is_floating_point<Float>::value, "theta must be float type.");
  return (theta > cast<Float>(kPi * 0.5))
      ? approxSin<Float, high_precision>(theta - cast<Float>(kPi * 1.5))
      : approxSin<Float, high_precision>(theta + cast<Float>(kPi * 0.5));
}

} // namespace zisc_math

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float sin(const Float theta)
{
  static_assert(kIsFloat<Float>, "Argument must be floating point.");
#ifndef ZISC_MATH_FAST_SINCOS
  return std::sin(theta);
#else // ZISC_MATH_FAST_SINCOS
  return zisc_math::approxSin(theta);
#endif // ZISC_MATH_FAST_SINCOS
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float cos(const Float theta)
{
  static_assert(kIsFloat<Float>, "Argument must be floating point.");
#ifndef ZISC_MATH_FAST_SINCOS
  return std::cos(theta);
#else // ZISC_MATH_FAST_SINCOS
  return zisc_math::approxCos(theta);
#endif // ZISC_MATH_FAST_SINCOS
}

namespace zisc_math {

/*!
  \details
  このアルゴリズムの詳細は "http://nghiaho.com/?p=997" を参照して下さい.

  \param[in] x 
  \return radian

  \attention 有効なxの範囲は [-1, 1] です
  */
template <typename Float> inline
constexpr Float approxArcTan(const Float x)
{
  static_assert(std::is_floating_point<Float>::value, "x must be float type.");
  return x * (cast<Float>(kPi * 0.25) - (isNegative(x)
    ? (-cast<Float>(1.0) - x) * (cast<Float>(0.2447) - cast<Float>(0.0663) * x)
    : (-cast<Float>(1.0) + x) * (cast<Float>(0.2447) + cast<Float>(0.0663) * x)));
}

/*!
  \details
  No detailed.

  \param[in] x 
  \return radian

  \attention 有効なxの範囲は [-1, 1] です
  */
template <typename Float> inline
/* constexpr */ Float approxArcCos(const Float x)
{
  static_assert(std::is_floating_point<Float>::value, "x must be float type.");
  return x < cast<Float>(0.0)
    ? cast<Float>(kPi) - cast<Float>(2.0) *
      approxArcTan((cast<Float>(1.0) + x) * invSqrt(cast<Float>(1.0) - x * x))
    : cast<Float>(2.0) * 
      approxArcTan((cast<Float>(1.0) - x) * invSqrt(cast<Float>(1.0) - x * x));
}

/*!
  \details
  No detailed.

  \param[in] x 
  \return radian

  \attention 有効なxの範囲は [-1, 1] です
  */
template <typename Float> inline
/* constexpr */ Float approxArcSin(const Float x)
{
  static_assert(std::is_floating_point<Float>::value, "x must be float type.");
  return cast<Float>(kPi * 0.5) - approxArcCos(x);
}

} // namespace zisc_math

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float arcSin(const Float x)
{
  static_assert(kIsFloat<Float>, "Argument must be floating point.");
#ifndef ZISC_MATH_FAST_ARCSINCOS
  return std::asin(x);
#else // ZISC_MATH_FAST_ARCSINCOS
  return zisc_math::approxArcSin(x);
#endif // ZISC_MATH_FAST_ARCSINCOS
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float arcCos(const Float x)
{
  static_assert(kIsFloat<Float>, "Argument must be floating point.");
#ifndef ZISC_MATH_FAST_ARCSINCOS
  return std::acos(x);
#else // ZISC_MATH_FAST_ARCSINCOS
  return zisc_math::approxArcCos(x);
#endif // ZISC_MATH_FAST_ARCSINCOS
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline 
Float arcTan(const Float x)
{
  static_assert(kIsFloat<Float>, "Argument must be floating point.");
#ifndef ZISC_MATH_FAST_ARCTAN
  return std::atan(x);
#else // ZISC_MATH_FAST_ARCTAN
  return zisc_math::approxArcTan(x);
#endif // ZISC_MATH_FAST_ARCTAN
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
std::tuple<std::array<Float, 2>, uint> solveQuadratic(
    const Float a, const Float b, const Float c)
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
Float solveCubicOneY(const Float p, const Float q)
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
    ZISC_VALUE_ASSERT(p < 0.0, p, "The p is positive.");
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
Float solveCubicOne(const Float b, const Float c, const Float d)
{
  static_assert(std::is_floating_point<Float>::value,
                "Float must be floating point type.");

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
Float solveCubicOne(const Float a, const Float b, const Float c, const Float d)
{
  static_assert(std::is_floating_point<Float>::value,
                "Float must be floating point type.");

  ZISC_ASSERT(a != 0.0, "The a must be non-zero value.");
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
                        std::array<Float, 4>& x)
{
  Float rest_max_x;
  if (x1[0] < x2[0]) {
    x[0] = x2[0];
    rest_max_x = x1[0];
  }
  else {
    x[0] = x1[0];
    rest_max_x = x2[0];
  }
  Float rest_min_x;
  if (x1[1] < x2[1]) {
    x[3] = x1[1];
    rest_min_x = x2[1];
  }
  else {
    x[3] = x2[1];
    rest_min_x = x1[1];
  }
  if (rest_min_x < rest_max_x) {
    x[1] = rest_max_x;
    x[2] = rest_min_x;
  }
  else {
    x[1] = rest_min_x;
    x[2] = rest_max_x;
  }
}

} // namespace zisc_math

/*!
  \details
  No detailed.
  */
template <typename Float> inline
std::tuple<std::array<Float, 4>, uint> solveQuartic(
    const Float a, const Float b, const Float c, const Float d, const Float e)
{
  static_assert(std::is_floating_point<Float>::value,
                "Float must be floating point type.");

  ZISC_ASSERT(a != 0.0, "The a must be non-zero value.");
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

#endif // _ZISC_MATH_INL_HPP_
