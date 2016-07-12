/*!
  \file dsfmt_engine-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_DSFMT_ENGINE_INL_HPP
#define ZISC_DSFMT_ENGINE_INL_HPP

#include "dsfmt_engine.hpp"
// Standard C++ library
#include <cstddef>
#include <cstdint>
#include <type_traits>
// Zisc
#include "error.hpp"
#include "utility.hpp"
#include "type_traits.hpp"

namespace zisc {

/*!
 \details
 No detailed.
 */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2> inline
DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::
DsfmtEngine() noexcept :
    index_{kN * 2},
    random_list_{&status_[0].d_[0]}
{
  setSeed(0);
}

/*!
 \details
 No detailed.
 */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2> inline
DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::
DsfmtEngine(const std::uint32_t seed) noexcept :
    index_{kN * 2},
    random_list_{&status_[0].d_[0]}
{
  setSeed(seed);
}

/*!
 \details
 No detailed.
 */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2> inline
double DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::
generate() noexcept
{
  if (index_ == (kN * 2)) {
    generateRandomAll();
    index_ = 0;
  }
  const auto random = random_list_[index_++];
  ZISC_ASSERT(isInClosedBounds(random, min(), max()), "The random is out of range.");
  return random;
}
  
/*!
 \details
 No detailed.
 */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2> 
template <typename Arithmetic> inline
Arithmetic 
    DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::
generate(const Arithmetic lower, const Arithmetic upper) noexcept
{
  static_assert(std::is_arithmetic<Arithmetic>::value,
                "Arithmetic isn't arithmetic type.");
  const auto random = generate();
  ZISC_ASSERT(isInClosedBounds(random, min(), max()), "The random is out of range.");
  return bound(random, lower, upper);
}

/*!
  \details
  No detailed.
  */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2> inline
constexpr double 
DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::max()
    noexcept
{
  return 2.0 - std::numeric_limits<double>::epsilon();
}

/*!
  \details
  No detailed.
  */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2> inline
constexpr double 
DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::min()
    noexcept
{
  return 1.0;
}

/*!
 \details
 No detailed.
 */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2> inline
void DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::
setSeed(const std::uint32_t seed) noexcept
{
  status_[0].u32_[0] = seed;
  initialize();
}

// private member function

/*!
  \details
  No detailed.
  */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2>
template <typename Float> inline
Float DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::
bound(const double random, 
      const Float lower, 
      const Float upper, 
      EnableIfFloat<Float>) noexcept
{
  return lower + cast<Float>(random - 1.0) * (upper - lower);
}

/*!
 \details
 No detailed.
 */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2>
void DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::
certificatePeriod() noexcept
{
  std::uint64_t tmp[2];
  tmp[0] = status_[kN].u64_[0] ^ kFix1;
  tmp[1] = status_[kN].u64_[1] ^ kFix2;

  std::uint64_t inner = (tmp[0] & kPcv1) ^ (tmp[1] & kPcv2);
  for (int i = 32; i > 0; i >>= 1)
    inner ^= inner >> i;
  inner &= 1;

  // check OK
  if (inner == 1)
    return;

  // check NG, and modification
  if ((kPcv2 & 1) == 1) {
    status_[kN].u64_[1] ^= 1;
  }
  else {
    std::uint64_t pcv[2] = {kPcv1, kPcv2};
    std::uint64_t work;
    for (std::size_t i = 2; i > 0; --i) {
      work = 1;
      for (int j = 0; j < 64; ++j) {
        if ((work & pcv[i - 1]) != 0) {
          status_[kN].u64_[i - 1] ^= work;
          return;
        }
        work = work << 1;
      }
    }
  }
}

/*!
 \details
 No detailed.
 */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2> inline
void DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::
generateRandom(const std::size_t destination, const std::size_t position) noexcept
{
  constexpr int kSr = 12;
  const std::uint64_t t0 = status_[destination].u64_[0],
                      t1 = status_[destination].u64_[1],
                      l0 = status_[kN].u64_[0],
                      l1 = status_[kN].u64_[1];
  status_[kN].u64_[0] = 
      (t0 << kSl) ^ (l1 >> 32) ^ (l1 << 32) ^ status_[position].u64_[0];
  status_[kN].u64_[1] = 
      (t1 << kSl) ^ (l0 >> 32) ^ (l0 << 32) ^ status_[position].u64_[1];
  status_[destination].u64_[0] = 
      (status_[kN].u64_[0] >> kSr) ^ (status_[kN].u64_[0] & kMask1) ^ t0;
  status_[destination].u64_[1] = 
      (status_[kN].u64_[1] >> kSr) ^ (status_[kN].u64_[1] & kMask2) ^ t1;
}

/*!
 \details
 No detailed.
 */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2>
void DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::
generateRandomAll() noexcept
{
  for (std::size_t i = 0; i < (kN - kPosition); ++i)
    generateRandom(i, i + kPosition);
  for (std::size_t i = (kN - kPosition); i < kN; ++i)
    generateRandom(i, i + kPosition - kN);
}

/*!
 \details
 No detailed.
 */
template <std::size_t kN, std::size_t kPosition, int kSl,
          std::uint64_t kMask1, std::uint64_t kMask2, std::uint64_t kFix1, 
          std::uint64_t kFix2, std::uint64_t kPcv1, std::uint64_t kPcv2>
void DsfmtEngine<kN, kPosition, kSl, kMask1, kMask2, kFix1, kFix2, kPcv1, kPcv2>::
initialize() noexcept
{
  constexpr std::uint64_t kHighConst = 0x3ff0000000000000;
  constexpr std::uint64_t kLowMask = 0x000fffffffffffff;

  std::uint32_t* u32_status = &status_[0].u32_[0];

  // Initialize dSFMT engin status.
  for (std::uint32_t i = 1; i < (kN + 1) * 4; ++i)
    u32_status[i] = 1812433253 * (u32_status[i - 1] ^ (u32_status[i - 1] >> 30)) + i;

  // Initialize mask
  std::uint64_t* u64_status = &status_[0].u64_[0];
  for (std::size_t i = 0; i < (kN * 2); ++i)
    u64_status[i] = (u64_status[i] & kLowMask) | kHighConst;

  certificatePeriod();
  index_ = kN * 2;
}

} // namespace zisc

#endif // ZISC_DSFMT_ENGINE_INL_HPP
