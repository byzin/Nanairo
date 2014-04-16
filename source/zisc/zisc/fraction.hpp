/*!
  \fraction.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_FRACTION_HPP_
#define _ZISC_FRACTION_HPP_

// 標準C++ライブラリ
#include <cstdint>
#include <string>
#include <type_traits>
// Ziscライブラリ
#include "type_traits.hpp"

namespace zisc {

/*!
 \brief 有理数を表します。
 \details
 値が負になる場合は，必ず分子にマイナスが付きます．
 */
template <typename SignedInteger>
class Fraction
{
  static_assert(kIsSignedInteger<SignedInteger>,
                "Fraction must be specified a signed integral type.");
          
 public:
  //! 0の有理数を作成します
  constexpr Fraction();

  //! 分母が1の有理数を作成します
  constexpr Fraction(const SignedInteger numerator);

  //! 有理数を作成します
  constexpr Fraction(const SignedInteger numerator, const SignedInteger denominator);


  Fraction& operator+=(const Fraction& fraction);

  Fraction& operator-=(const Fraction& fraction);

  Fraction& operator*=(const Fraction& fraction);

  Fraction& operator/=(const Fraction& fraction);


  //! 浮動小数点に変換した値を取得します
  template <typename FloatingPoint>
  constexpr FloatingPoint toFloat() const;

  //! 分母の絶対値を取得します
  constexpr SignedInteger getDenominator() const;

  //! 分子の絶対値を取得します
  constexpr SignedInteger getNumerator() const;

 private:
  SignedInteger numerator_,
                denominator_;
};

// よく使う型に別名をつけます
using Fraction16 = Fraction<std::int16_t>;
using Fraction32 = Fraction<std::int32_t>;
using Fraction64 = Fraction<std::int64_t>;

template <typename SignedInteger>
constexpr Fraction<SignedInteger> operator+(const Fraction<SignedInteger>& a,
                                            const Fraction<SignedInteger>& b);

template <typename SignedInteger>
constexpr Fraction<SignedInteger> operator-(const Fraction<SignedInteger>& a,
                                            const Fraction<SignedInteger>& b);

template <typename SignedInteger>
constexpr Fraction<SignedInteger> operator*(const Fraction<SignedInteger>& a,
                                            const Fraction<SignedInteger>& b);

template <typename SignedInteger>
constexpr Fraction<SignedInteger> operator/(const Fraction<SignedInteger>& a,
                                            const Fraction<SignedInteger>& b);

//! a と b の有理数が等しいか判定します
template <typename SignedInteger>
constexpr bool operator==(const Fraction<SignedInteger>& a,
                          const Fraction<SignedInteger>& b);

//! a と b が等しくないか判定します
template <typename SignedInteger>
constexpr bool operator!=(const Fraction<SignedInteger>& a,
                          const Fraction<SignedInteger>& b);

//! a が b よりも小さいか判定します
template <typename SignedInteger>
constexpr bool operator<(const Fraction<SignedInteger>& a,
                         const Fraction<SignedInteger>& b);

//! a が b 以下か判定します
template <typename SignedInteger>
constexpr bool operator<=(const Fraction<SignedInteger>& a,
                          const Fraction<SignedInteger>& b);

//! a が b よりも大きいか判定します
template <typename SignedInteger>
constexpr bool operator>(const Fraction<SignedInteger>& a,
                         const Fraction<SignedInteger>& b);

//! a が b 以上か判定します
template <typename SignedInteger>
constexpr bool operator>=(const Fraction<SignedInteger>& a,
                          const Fraction<SignedInteger>& b);

} // namespace zisc

#include "fraction-inl.hpp"

namespace zisc {

constexpr Fraction32 kPiFraction{355, 113}; //! 円周率

} // namespace zisc

#endif // _ZISC_FRACTION_HPP_
