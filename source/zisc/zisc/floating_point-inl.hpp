/*!
  \file floating_point-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_FLOATING_POINT_INL_HPP
#define ZISC_FLOATING_POINT_INL_HPP

#include "floating_point.hpp"
// Zisc
#include "utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

template <>
struct FloatingPointUtility<float>
{
  using BitType = uint32;
};

template <>
struct FloatingPointUtility<double>
{
  using BitType = uint64;
};

/*!
  */
template <> inline
constexpr auto FloatBit::mantissaBitSize() noexcept -> BitType
{
  constexpr BitType size = 23;
  return size;
}

/*!
  */
template <> inline
constexpr auto DoubleBit::mantissaBitSize() noexcept -> BitType
{
  constexpr BitType size = 52;
  return size;
}

/*!
  */
template <> inline
constexpr auto FloatBit::exponentSignBitMask() noexcept -> BitType
{
  constexpr BitType shift_length = mantissaBitSize();
  constexpr BitType bit_mask = 0b1000'0000u << shift_length;
  return bit_mask;
}

/*!
  */
template <> inline
constexpr auto DoubleBit::exponentSignBitMask() noexcept -> BitType
{
  constexpr BitType shift_length = mantissaBitSize();
  constexpr BitType bit_mask = 0b0100'0000'0000ull << shift_length;
  return bit_mask;
}

/*!
  */
template <> inline
constexpr auto FloatBit::exponentValueBitMask() noexcept -> BitType
{
  constexpr BitType shift_length = mantissaBitSize();
  constexpr BitType bit_mask = 0b0111'1111u << shift_length;
  return bit_mask;
}

/*!
  */
template <> inline
constexpr auto DoubleBit::exponentValueBitMask() noexcept -> BitType
{
  constexpr BitType shift_length = mantissaBitSize();
  constexpr BitType bit_mask = 0b0011'1111'1111ull << shift_length;
  return bit_mask;
}

/*!
  */
template <typename Float> inline
constexpr auto FloatingPointBit<Float>::exponentBitMask() noexcept -> BitType
{
  return exponentSignBitMask() | exponentValueBitMask();
}

/*!
  */
template <> inline
constexpr auto FloatBit::exponentBitSize() noexcept -> BitType
{
  constexpr BitType size = 8;
  return size;
}

/*!
  */
template <> inline
constexpr auto DoubleBit::exponentBitSize() noexcept -> BitType
{
  constexpr BitType size = 11;
  return size;
}

/*!
  */
template <typename Float> inline
constexpr auto FloatingPointBit<Float>::getExponentBits(
    const Float value) noexcept -> BitType
{
  const BitF v{value};
  return (exponentBitMask() & v.bit_);
}

/*!
  */
template <typename Float> inline
constexpr auto FloatingPointBit<Float>::getMantissaBits(
    const Float value) noexcept -> BitType
{
  const BitF v{value};
  return (mantissaBitMask() & v.bit_);
}

/*!
  */
template <typename Float> inline
constexpr auto FloatingPointBit<Float>::getSignBit(
    const Float value) noexcept -> BitType
{
  const BitF v{value};
  return (signBitMask() & v.bit_);
}

/*!
  */
template <typename Float> inline
constexpr auto FloatingPointBit<Float>::halfExponentBits(
    const BitType exponent_bits) noexcept -> BitType
{
  constexpr BitType sign_bit_mask = exponentSignBitMask();
  constexpr BitType value_bit_mask = exponentValueBitMask();
  const auto e = (isPositiveExponent(exponent_bits)) ? exponent_bits - 1
                                                     : exponent_bits;
  const auto sign_bit = e & sign_bit_mask;
  const auto value_bits = ((sign_bit ^ sign_bit_mask) | (e & value_bit_mask)) >> 1;
  return (sign_bit | value_bits) & exponentBitMask();
}

/*!
  */
template <typename Float> inline
constexpr bool FloatingPointBit<Float>::isOddExponent(const Float value) noexcept
{
  const BitF v{value};
  return isOddExponent(v.bit_);
}

/*!
  */
template <typename Float> inline
constexpr bool FloatingPointBit<Float>::isOddExponent(
    const BitType exponent_bits) noexcept
{
  constexpr BitType shift_length = mantissaBitSize();
  constexpr BitType bit_mask = cast<BitType>(0b1u) << shift_length;
  return (exponent_bits & bit_mask) != bit_mask;
}

/*!
  */
template <typename Float> inline
constexpr bool FloatingPointBit<Float>::isPositiveExponent(
    const Float value) noexcept
{
  const BitF v{value};
  return isPositiveExponent(v.bit_);
}

/*!
  */
template <typename Float> inline
constexpr bool FloatingPointBit<Float>::isPositiveExponent(
    const BitType exponent_bits) noexcept
{
  constexpr BitType sign_bit_mask = exponentSignBitMask();
  return (exponent_bits & sign_bit_mask) == sign_bit_mask;
}

/*!
  */
template <typename Float> inline
constexpr auto FloatingPointBit<Float>::makeFloat(
    const BitType mantissa_bits, 
    const BitType exponent_bits) noexcept -> Float
{
  const BitF v{mantissa_bits | exponent_bits};
  return v.float_;
}

/*!
  */
template <typename Float> inline
constexpr auto FloatingPointBit<Float>::makeFloat(
    const BitType sign_bit,
    const BitType mantissa_bits, 
    const BitType exponent_bits) noexcept -> Float
{
  const BitF v{sign_bit | mantissa_bits | exponent_bits};
  return v.float_;
}

/*!
  */
template <> inline
constexpr auto FloatBit::mantissaBitMask() noexcept -> BitType
{
  constexpr BitType bit_mask = 0x7fffffu;
  return bit_mask;
}

/*!
  */
template <> inline
constexpr auto DoubleBit::mantissaBitMask() noexcept -> BitType
{
  constexpr BitType bit_mask = 0xfffffffffffffull;
  return bit_mask;
}

/*!
  */
template <typename Float> inline
constexpr auto FloatingPointBit<Float>::signBitMask() noexcept -> BitType
{
  constexpr BitType shift_length = mantissaBitSize() + exponentBitSize();
  constexpr BitType bit_mask = cast<BitType>(0b1u) << shift_length;
  return bit_mask;
}

/*!
  */
template <typename Float> inline
constexpr auto FloatingPointBit<Float>::signBitSize() noexcept -> BitType
{
  constexpr BitType size = 1;
  return size;
}

} // namespace zisc

#endif // ZISC_FLOATING_POINT_INL_HPP
