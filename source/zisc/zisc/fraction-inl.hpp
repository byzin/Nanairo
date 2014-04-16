/*!
  \file fraction-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_FRACTION_INL_HPP_
#define _ZISC_FRACTION_INL_HPP_

#include "fraction.hpp"
// 標準C++ライブラリ
#include <string>
// Ziscライブラリ
#include "error.hpp"
#include "math.hpp"
#include "string.hpp"
#include "utility.hpp"

namespace zisc {

// public member function

/*!
  \details
  No detailed.
  */
template <typename SignedInteger> inline
constexpr Fraction<SignedInteger>::Fraction() :
    numerator_{cast<SignedInteger>(0)},
    denominator_{cast<SignedInteger>(1)}
{
}

/*!
  \details
  No detailed.

  \param[in] numerator 分子の値
  */
template <typename SignedInteger> inline
constexpr Fraction<SignedInteger>::Fraction(const SignedInteger numerator) :
        numerator_{numerator},
        denominator_{cast<SignedInteger>(1)}
{
}

/*!
  \details
  No detailed.

  \param[in] numerator 分子の値
  \param[in] denominator 分母の値
  */
template <typename SignedInteger> inline
constexpr Fraction<SignedInteger>::Fraction(const SignedInteger numerator, 
                                            const SignedInteger denominator) :
      numerator_{(isNegative(denominator) ? -numerator : numerator) /
                     gcd(abs(numerator), abs(denominator))},
      denominator_{abs(denominator) / gcd(abs(numerator), abs(denominator))}
{
}

template <typename SignedInteger> inline
Fraction<SignedInteger>& Fraction<SignedInteger>::operator+=(const Fraction& fraction)
{
  return *this = (*this + fraction);
}

template <typename SignedInteger> inline
Fraction<SignedInteger>& Fraction<SignedInteger>::operator-=(const Fraction& fraction)
{
  return *this = (*this - fraction);
}

template <typename SignedInteger> inline
Fraction<SignedInteger>& Fraction<SignedInteger>::operator*=(const Fraction& fraction)
{
  return *this = (*this * fraction);
}

template <typename SignedInteger> inline
Fraction<SignedInteger>& Fraction<SignedInteger>::operator/=(const Fraction& fraction)
{
  return *this = (*this / fraction);
}

/*!
 \details
 No detailed.

 \return 変換した浮動小数点
 */
template <typename SignedInteger> template <typename FloatingPoint> inline
constexpr FloatingPoint Fraction<SignedInteger>::toFloat() const
{
  return cast<FloatingPoint>(numerator_) / cast<FloatingPoint>(denominator_);
}

/*!
 \details
 No detailed.

 \return 分母の絶対値
 */
template <typename SignedInteger> inline
constexpr SignedInteger Fraction<SignedInteger>::getDenominator() const
{
  return denominator_;
}

/*!
 \details
 No detailed.

 \return 分子の絶対値
 */
template <typename SignedInteger> inline
constexpr SignedInteger Fraction<SignedInteger>::getNumerator() const
{
  return numerator_;
}

template <typename SignedInteger> inline
constexpr Fraction<SignedInteger> operator+(const Fraction<SignedInteger>& a,
                                            const Fraction<SignedInteger>& b)
{
  return Fraction<SignedInteger>{a.getNumerator() * b.getDenominator() +
                                 a.getDenominator() * b.getNumerator(),
                                 a.getDenominator() * b.getDenominator()};
}

template <typename SignedInteger> inline
constexpr Fraction<SignedInteger> operator-(const Fraction<SignedInteger>& a,
                                            const Fraction<SignedInteger>& b)
{
  return Fraction<SignedInteger>{a.getNumerator() * b.getDenominator() -
                                 a.getDenominator() * b.getNumerator(),
                                 a.getDenominator() * b.getDenominator()};
}

template <typename SignedInteger> inline
constexpr Fraction<SignedInteger> operator*(const Fraction<SignedInteger>& a,
                                            const Fraction<SignedInteger>& b)
{
  return Fraction<SignedInteger>{a.getNumerator() * b.getNumerator(),
                                 a.getDenominator() * b.getDenominator()};
}

template <typename SignedInteger> inline
constexpr Fraction<SignedInteger> operator/(const Fraction<SignedInteger>& a,
                                            const Fraction<SignedInteger>& b)
{
  return Fraction<SignedInteger>{a.getNumerator() * b.getDenominator(),
                                 a.getDenominator() * b.getNumerator()};
}

/*!
 \details
 No detailed.

 \param[in] a 有理数
 \param[in] b 有理数
 \return a と b が等しい場合はtrue、そうでない場合はfalse
 */
template <typename SignedInteger> inline
constexpr bool operator==(const Fraction<SignedInteger>& a,
                          const Fraction<SignedInteger>& b)
{
  return (a.getNumerator() == b.getNumerator()) &&
         (a.getDenominator() == b.getDenominator());
}

/*!
 \details
 No detailed.

 \param[in] a 有理数
 \param[in] b 有理数
 \return a と b が等しくない場合はtrue、そうでない場合はfalse
 */
template <typename SignedInteger> inline
constexpr bool operator!=(const Fraction<SignedInteger>& a,
                          const Fraction<SignedInteger>& b)
{
  return !(a == b);
}

/*!
 \details
 No detailed.

 \param[in] a 有理数
 \param[in] b 有理数
 \return a が b よりも小さい場合はtrue、そうでない場合はfalse
 */
template <typename SignedInteger> inline
constexpr bool operator<(const Fraction<SignedInteger>& a,
                         const Fraction<SignedInteger>& b)
{
  return a.getNumerator()*b.getDenominator() < a.getDenominator()*b.getNumerator();
}

/*!
 \details
 No detailed.

 \param[in] a 有理数
 \param[in] b 有理数
 \return a が b 以下の場合はtrue、そうでない場合はfalse
 */
template <typename SignedInteger> inline
constexpr bool operator<=(const Fraction<SignedInteger>& a,
                          const Fraction<SignedInteger>& b)
{
  return (a == b) || (a < b);
}

/*!
 \details
 No detailed.

 \param[in] a 有理数
 \param[in] b 有理数
 \return a が b よりも大きい場合はtrue、そうでない場合はfalse
 */
template <typename SignedInteger> inline
constexpr bool operator>(const Fraction<SignedInteger>& a,
                         const Fraction<SignedInteger>& b)
{
  return b < a;
}

/*!
 \details
 No detailed.

 \param[in] a 有理数
 \param[in] b 有理数
 \return a が b 以上の場合はtrue、そうでない場合はfalse
 */
template <typename SignedInteger> inline
constexpr bool operator>=(const Fraction<SignedInteger>& a,
                          const Fraction<SignedInteger>& b)
{
  return b <= a;
}

} // namespace zisc

#endif // _ZISC_FRACTION_INL_HPP_
