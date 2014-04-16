/*!
  \file xorshift_engine-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_XORSHIFT_ENGINE_INL_HPP_
#define _ZISC_XORSHIFT_ENGINE_INL_HPP_

#include "xorshift_engine.hpp"
// Standard C++ library
#include <cstdint>
#include <limits>
#include <type_traits>
// Zisc
#include "pseudo_random_number_engine.hpp"
#include "type_traits.hpp"
#include "utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
 \details
 No detailed.
 */
template <typename Type> inline
XorshiftEngine<Type>::XorshiftEngine()
{
  setSeed(0);
}

/*!
 \details
 No detailed.
 */
template <typename Type> inline
XorshiftEngine<Type>::XorshiftEngine(const SeedType seed)
{
  setSeed(seed);
}

/*!
 \details
 No detailed.
 */
template <> inline
auto Xorshift32::generate() -> ResultType
{
  const uint32 temp = seed_[0] ^ (seed_[0] << 11);
  seed_[0] = seed_[1];
  seed_[1] = seed_[2];
  seed_[2] = seed_[3];
  seed_[3] = (seed_[3] ^ (seed_[3] >> 19)) ^ (temp ^ (temp >> 8));
  return seed_[3];
}

/*!
 \details
 No detailed.
 */
template <> inline
auto Xorshift128Plus::generate() -> ResultType
{
  uint64 s1 = seed_[0];
  const uint64 s0 = seed_[1];
  s1 ^= s1 << 23;
  s1 = s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26);
  seed_[0] = s0;
  seed_[1] = s1;
  return s0 + s1;
}

/*!
  \details
  No detailed.
  */
template <typename Type> template <typename Arithmetic> inline
Arithmetic XorshiftEngine<Type>::generate(const Arithmetic lower, 
                                          const Arithmetic upper)
{
  static_assert(std::is_arithmetic<Arithmetic>::value,
                "## Arguments must be arithmetic type.");
  const auto random = generate();
  return bound(random, lower, upper);
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
constexpr auto XorshiftEngine<Type>::max() -> ResultType
{
  return std::numeric_limits<ResultType>::max();
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
constexpr auto XorshiftEngine<Type>::min() -> ResultType
{
  return std::numeric_limits<ResultType>::min();
}

/*!
 \details
 No detailed.
 */
template <> inline
void Xorshift32::setSeed(const SeedType seed)
{
  seed_[0] = 0x498b3bc5 ^ seed;
  seed_[1] = 0;
  seed_[2] = 0;
  seed_[3] = 0;
}

/*!
  \details
  No detailed.
  */
template <> inline
void Xorshift128Plus::setSeed(const SeedType seed)
{
  auto state = treatAs<uint32*>(seed_);
  state[0] = 0x498b3bc5 ^ cast<uint32>(seed >>  0);
  state[1] = 0x5a05089a ^ cast<uint32>(seed >> 32);
  state[2] = 0;
  state[3] = 0;
}

/*!
  \details
  No detailed.
  */
template <typename Type> template <typename Float> inline
Float XorshiftEngine<Type>::bound(const ResultType random, 
                                  const Float lower, 
                                  const Float upper,
                                  EnableIfFloat<Float>) const
{
  constexpr Float epsilon = std::numeric_limits<Float>::epsilon();
  constexpr Float normalizer = (cast<Float>(1.0) - epsilon) / cast<Float>(max());
  const Float u = cast<Float>(random) * normalizer;
  return lower + u * (upper - lower);
}

} // namespace zisc

#endif // _ZISC_XORSHIFT_ENGINE_INL_HPP_
