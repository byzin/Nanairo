/*!
  \file sample_statistics-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLE_STATISTICS_INL_HPP
#define NANAIRO_SAMPLE_STATISTICS_INL_HPP

#include "sample_statistics.hpp"
// Standard C++ library
#include <cstddef>
#include <bitset>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/SpectralDistribution/spectral_distribution.hpp"
#include "NanairoCore/Geometry/point.hpp"

namespace nanairo {

/*!
  */
inline
uint SampleStatistics::getIndex(const Index2d position) const noexcept
{
  const uint index = position[1] * resolution()[0] + position[0];
  return index;
}

/*!
  */
inline
SpectralDistribution& SampleStatistics::getSample(
    const Index2d position) noexcept
{
  auto& table = sampleTable();
  const uint index = getIndex(position);
  return *table[index];
}

/*!
  */
inline
const SpectralDistribution& SampleStatistics::getSample(
    const Index2d position) const noexcept
{
  const auto& table = sampleTable();
  const uint index = getIndex(position);
  return *table[index];
}

/*!
  */
inline
SpectralDistribution& SampleStatistics::getSampleSquared(
    const Index2d position) noexcept
{
  auto& table = sampleSquaredTable();
  const uint index = getIndex(position);
  return *table[index];
}

/*!
  */
inline
const SpectralDistribution& SampleStatistics::getSampleSquared(
    const Index2d position) const noexcept
{
  const auto& table = sampleSquaredTable();
  const uint index = getIndex(position);
  return *table[index];
}

/*!
  */
inline
bool SampleStatistics::isEnabled(const Type type) const noexcept
{
  const std::size_t index = zisc::cast<std::size_t>(type);
  return flag_[index];
}

/*!
  */
inline
Index2d SampleStatistics::resolution() const noexcept
{
  return resolution_;
}

/*!
  */
inline
auto SampleStatistics::sampleTable() noexcept
    -> zisc::pmr::vector<SpectralDistributionPointer>&
{
  ZISC_ASSERT(isEnabled(Type::kExpectedValue), "The flag isn't enabled.");
  return sample_;
}

/*!
  */
inline
auto SampleStatistics::sampleTable() const noexcept
    -> const zisc::pmr::vector<SpectralDistributionPointer>&
{
  ZISC_ASSERT(isEnabled(Type::kExpectedValue), "The flag isn't enabled.");
  return sample_;
}

/*!
  */
inline
auto SampleStatistics::sampleSquaredTable() noexcept
    -> zisc::pmr::vector<SpectralDistributionPointer>&
{
  ZISC_ASSERT(isEnabled(Type::kVariance), "The flag isn't enabled.");
  return sample_squared_;
}

/*!
  */
inline
auto SampleStatistics::sampleSquaredTable() const noexcept
    -> const zisc::pmr::vector<SpectralDistributionPointer>&
{
  ZISC_ASSERT(isEnabled(Type::kVariance), "The flag isn't enabled.");
  return sample_squared_;
}

} // namespace nanairo

#endif // NANAIRO_SAMPLE_STATISTICS_INL_HPP
