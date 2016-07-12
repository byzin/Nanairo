/*!
  \fraction.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_FRACTION_HPP
#define ZISC_FRACTION_HPP

// Standard C++ library
#include <type_traits>
// Zisc
#include "type_traits.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
 */
template <typename SignedInteger>
class Fraction
{
  static_assert(kIsSignedInteger<SignedInteger>,
                "Fraction must be specified a signed integral type.");
          
 public:
  //! Creata a fraction (0 / 1)
  constexpr Fraction() noexcept;

  //! Create a fraction (numerator / 1)
  constexpr Fraction(const SignedInteger n) noexcept;

  //! Create a fraction (numerator / denominator)
  constexpr Fraction(const SignedInteger n, const SignedInteger d) noexcept;


  //!
  Fraction& operator+=(const Fraction& other) noexcept;

  //!
  Fraction& operator-=(const Fraction& other) noexcept;

  //!
  Fraction& operator*=(const Fraction& other) noexcept;

  //!
  Fraction& operator/=(const Fraction& other) noexcept;


  //! Return the denominator
  constexpr SignedInteger denominator() const noexcept;

  //! Return the numerator 
  constexpr SignedInteger numerator() const noexcept;

  //! Return a inverse fraction
  constexpr Fraction invert() const noexcept;

  //! Multiplay two fractions
  constexpr Fraction multiply(const Fraction& other) const noexcept;

  //! Convert a fraction to a float
  template <typename Float>
  constexpr Float toFloat() const noexcept;

 private:
  //! Return the absolute value
  static constexpr SignedInteger abs(const SignedInteger n) noexcept;

  //! Return the greatest common divisor of two integer m and n
  static constexpr SignedInteger gcd(SignedInteger m, SignedInteger n) noexcept;


  SignedInteger numerator_,
                denominator_;
};

// Aliases
using Fraction16 = Fraction<int16>;
using Fraction32 = Fraction<int32>;
using Fraction64 = Fraction<int64>;

//! 
template <typename SignedInteger>
constexpr Fraction<SignedInteger> operator+(
    const Fraction<SignedInteger>& lhs,
    const Fraction<SignedInteger>& rhs) noexcept;
 
//!
template <typename SignedInteger>
constexpr Fraction<SignedInteger> operator-(
    const Fraction<SignedInteger>& lhs,
    const Fraction<SignedInteger>& rhs) noexcept;

//!
template <typename SignedInteger>
constexpr Fraction<SignedInteger> operator*(
    const Fraction<SignedInteger>& lhs,
    const Fraction<SignedInteger>& rhs) noexcept;

//!
template <typename SignedInteger>
constexpr Fraction<SignedInteger> operator/(
    const Fraction<SignedInteger>& lhs,
    const Fraction<SignedInteger>& rhs) noexcept;

//! Check if two fractions are equal in value
template <typename SignedInteger>
constexpr bool operator==(
    const Fraction<SignedInteger>& lhs,
    const Fraction<SignedInteger>& rhs) noexcept;

//! Check if two fractions aren't equal in value
template <typename SignedInteger>
constexpr bool operator!=(
    const Fraction<SignedInteger>& lhs,
    const Fraction<SignedInteger>& rhs) noexcept;

//!
template <typename SignedInteger>
constexpr bool operator<(
    const Fraction<SignedInteger>& lhs,
    const Fraction<SignedInteger>& rhs) noexcept;

//!
template <typename SignedInteger>
constexpr bool operator<=(
    const Fraction<SignedInteger>& lhs,
    const Fraction<SignedInteger>& rhs) noexcept;

//!
template <typename SignedInteger>
constexpr bool operator>(
    const Fraction<SignedInteger>& lhs,
    const Fraction<SignedInteger>& rhs) noexcept;

//!
template <typename SignedInteger>
constexpr bool operator>=(
    const Fraction<SignedInteger>& lhs,
    const Fraction<SignedInteger>& rhs) noexcept;

} // namespace zisc

#include "fraction-inl.hpp"

namespace zisc {

constexpr Fraction32 kPiFraction{355, 113};

} // namespace zisc

#endif // ZISC_FRACTION_HPP
