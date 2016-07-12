/*!
  \file xsadd_engine-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_XSADD_ENGINE_INL_HPP
#define ZISC_XSADD_ENGINE_INL_HPP

#include "xsadd_engine.hpp"
// Standard C++ library
#include <cstdint>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <vector>
// Zisc
#include "error.hpp"
#include "pseudo_random_number_engine.hpp"
#include "utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
inline
XsaddEngine::XsaddEngine() noexcept
{
  setSeed(0);
}

/*!
  \details
  No detailed.
  */
inline
XsaddEngine::XsaddEngine(const uint32 seed) noexcept
{
  setSeed(seed);
}

/*!
  \details
  No detailed.
  */
inline
uint32 XsaddEngine::generate() noexcept
{
  nextState();
  const auto random = state_[3] + state_[2];
  ZISC_ASSERT(isInClosedBounds(random, min(), max()), "The random is out of range.");
  return random;
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float XsaddEngine::generate(const Float lower, 
                            const Float upper,
                            EnableIfFloat<Float>) noexcept
{
  const auto random = generateFloat();
  return bound(random, lower, upper);
}

/*!
  \details
  No detailed.
  */
inline
double XsaddEngine::generateFloat() noexcept
{
  constexpr double k = 1.0 / 18446744073709551616.0;
  constexpr uint32 t = 0xffffffe0u;

  uint64 a = cast<uint64>(generate() & t);
  const uint64 b = cast<uint64>(generate());
	a = (a << 32) | (b << 5);
	return k * cast<double>(a);
}

/*!
  \details
  No detailed.
  */
inline
constexpr uint32 XsaddEngine::max() noexcept
{
  return std::numeric_limits<uint32>::max();
}

/*!
  \details
  No detailed.
  */
inline
constexpr uint32 XsaddEngine::min() noexcept
{
  return std::numeric_limits<uint32>::min();
}

/*!
  \details
  No detailed.
  */
void XsaddEngine::setSeed(const uint32 seed) noexcept
{
  state_[0] = seed;
  state_[1] = 0;
  state_[2] = 0;
  state_[3] = 0;

  for (uint i = 1; i < loop_; ++i) {
    constexpr uint32 t = 1812433253u;
    state_[i & 3] ^= i + t * (state_[(i - 1) & 3] ^ (state_[(i - 1) & 3] >> 30));
  }
  periodCertification();
  for (uint i = 0; i < loop_; ++i)
    nextState();
}

/*!
  \details
  No detailed.
  */
void XsaddEngine::setSeed(const std::vector<uint32>& seed_array) noexcept
{
  constexpr int lag = 1;
  constexpr int mid = 1;
  constexpr int size = 4;

  state_[0] = 0;
  state_[1] = 0;
  state_[2] = 0;
  state_[3] = 0;

  using SizeType = 
      typename std::remove_reference<decltype(seed_array)>::type::size_type;
  SizeType count;
  if (seed_array.size() + 1 > loop_)
    count = seed_array.size() + 1;
  else
    count = loop_;
  uint32 r = initializeValue1(state_[0] ^ 
                                     state_[mid % size] ^ 
                                     state_[(size - 1) % size]);
  state_[mid % size] += r;
  r += seed_array.size();
  state_[(mid + lag) % size] += r;
  state_[0] = r;
  --count;
  uint i, j;
  for (i = 1, j = 0; (j < count) && (j < seed_array.size()); ++j) {
    r = initializeValue1(state_[i % size] ^ 
                         state_[(i + mid) % size] ^ 
                         state_[(i + size - 1) % size]);
    state_[(i + mid) % size] += r;
    r += seed_array[j] + i;
    state_[(i + mid + lag) % size] += r;
    state_[i % size] = r;
    i = (i + 1) % size;
  }
  for (; j < count; ++j) {
    r = initializeValue1(state_[i % size] ^
                         state_[(i + mid) % size] ^
                         state_[(i + size - 1) % size]);
    state_[(i + mid) % size] += r;
    r += i;
    state_[(i + mid + lag) % size] += r;
    state_[i % size] = r;
    i = (i + 1) % size;
  }
  for (j = 0; j < size; ++j) {
    r = initializeValue2(state_[i % size] +
                         state_[(i + mid) % size] +
                         state_[(i + size - 1) % size]);
    state_[(i + mid) % size] ^= r;
    r -= i;
    state_[(i + mid + lag) % size] ^= r;
    state_[i % size] = r;
    i = (i + 1) % size;
  }
  periodCertification();

  for (i = 0; i < loop_; ++i)
    nextState();
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float XsaddEngine::bound(const double random, 
                         const Float lower, 
                         const Float upper,
                         EnableIfFloat<Float>) noexcept
{
  return lower + cast<Float>(random) * (upper - lower);
}

/*!
  \details
  No detailed.
  */
inline
uint32 XsaddEngine::initializeValue1(const uint32 x) const noexcept
{
  constexpr uint32 t = 1664525u;
  return (x ^ (x >> 27)) * t;
}

/*!
  \details
  No detailed.
  */
inline
uint32 XsaddEngine::initializeValue2(const uint32 x) const noexcept
{
  constexpr uint32 t = 1566083941u;
  return (x ^ (x >> 27)) * t;
}

/*!
  \details
  No detailed.
  */
inline
void XsaddEngine::nextState() noexcept
{
  constexpr int sh1 = 15;
  constexpr int sh2 = 18;
  constexpr int sh3 = 11;

  uint32 t = state_[0];
  t ^= t << sh1;
  t ^= t >> sh2;
  t ^= state_[3] << sh3;
  state_[0] = state_[1];
  state_[1] = state_[2];
  state_[2] = state_[3];
  state_[3] = t;
}

/*!
  \details
  No detailed.
  */
inline
void XsaddEngine::periodCertification() noexcept
{
  if (state_[0] == 0 && state_[1] == 0 && state_[2] == 0 && state_[3] == 0) {
    state_[0] = 'X';
    state_[1] = 'S';
    state_[2] = 'A';
    state_[3] = 'D';
  }
}

} // namespace zisc

#endif // ZISC_XSADD_ENGINE_INL_HPP
