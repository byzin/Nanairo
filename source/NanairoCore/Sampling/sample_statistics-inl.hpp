/*!
  \file sample_statistics-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
#include "zisc/compensated_summation.hpp"
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
auto SampleStatistics::covarianceFactorTable() noexcept
    -> zisc::pmr::vector<zisc::CompensatedSummation<Float>>&
{
  ZISC_ASSERT(isEnabled(Type::kDenoisedExpectedValue), "The flag isn't enabled.");
  return covariance_factor_;
}

/*!
  */
inline
auto SampleStatistics::covarianceFactorTable() const noexcept
    -> const zisc::pmr::vector<zisc::CompensatedSummation<Float>>&
{
  ZISC_ASSERT(isEnabled(Type::kDenoisedExpectedValue), "The flag isn't enabled.");
  return covariance_factor_;
}

/*!
  */
inline
auto SampleStatistics::denoisedSampleTable() noexcept
    -> zisc::pmr::vector<SpectralDistributionPointer>&
{
  ZISC_ASSERT(isEnabled(Type::kDenoisedExpectedValue), "The flag isn't enabled.");
  return denoised_sample_;
}

/*!
  */
inline
auto SampleStatistics::denoisedSampleTable() const noexcept
    -> const zisc::pmr::vector<SpectralDistributionPointer>&
{
  ZISC_ASSERT(isEnabled(Type::kDenoisedExpectedValue), "The flag isn't enabled.");
  return denoised_sample_;
}

/*!
  */
inline
uint SampleStatistics::getFactorIndex(const uint i) const noexcept
{
  const uint size = sampleTable()[0]->size();
  const uint index = (i * (2 * size - (i + 1))) >> 1;
  return index;
}

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
bool SampleStatistics::isEnabled(const Type type) const noexcept
{
  const std::size_t index = zisc::cast<std::size_t>(type);
  return flag_[index];
}

/*!
  */
inline
auto SampleStatistics::histogramTable() noexcept
    -> zisc::pmr::vector<SpectralDistributionPointer>&
{
  ZISC_ASSERT(isEnabled(Type::kDenoisedExpectedValue), "The flag isn't enabled.");
  return histogram_;
}

/*!
  */
inline
auto SampleStatistics::histogramTable() const noexcept
    -> const zisc::pmr::vector<SpectralDistributionPointer>&
{
  ZISC_ASSERT(isEnabled(Type::kDenoisedExpectedValue), "The flag isn't enabled.");
  return histogram_;
}

/*!
  */
inline
uint SampleStatistics::numOfCovarianceFactors() const noexcept
{
  const uint size = sampleTable()[0]->size();
  return getFactorIndex(size);
}

/*!
  */
inline
auto SampleStatistics::prevSampleTable() noexcept
    -> zisc::pmr::vector<SpectralDistributionPointer>&
{
  ZISC_ASSERT(isEnabled(Type::kVariance), "The flag isn't enabled.");
  return prev_sample_;
}

/*!
  */
inline
auto SampleStatistics::prevSampleTable() const noexcept
    -> const zisc::pmr::vector<SpectralDistributionPointer>&
{
  ZISC_ASSERT(isEnabled(Type::kVariance), "The flag isn't enabled.");
  return prev_sample_;
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
