/*!
  \file spectral_distribution.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "spectral_distribution.hpp"
// Standard C++ library
#include <cmath>
#include <utility>
// Zisc
#include "zisc/compensated_summation.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "spectral_distribution_rgb.hpp"
#include "spectral_distribution_spectra.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Setting/spectra_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  */
SpectralDistribution::~SpectralDistribution() noexcept
{
}

/*!
  */
void SpectralDistribution::clampAll(const Float minimum,
                                    const Float maximum) noexcept
{
  for (uint i = 0; i < size(); ++i) {
    const Float v = zisc::clamp(get(i), minimum, maximum);
    set(i, v);
  }
}

/*!
  */
void SpectralDistribution::correctGamma(const Float gamma) noexcept
{
  for (uint i = 0; i < size(); ++i) {
    const Float v = zisc::pow(get(i), gamma);
    set(i, v);
  }
}

/*!
  */
void SpectralDistribution::fill(const Float value) noexcept
{
  for (uint i = 0; i < size(); ++i)
    set(i, value);
}

/*!
  */
bool SpectralDistribution::hasInf() const noexcept
{
  bool result = false;
  for (uint i = 0; (i < size()) && !result; ++i)
    result = zisc::isInf(get(i));
  return result;
}

/*!
  */
bool SpectralDistribution::hasNan() const noexcept
{
  bool result = false;
  for (uint i = 0; (i < size()) && !result; ++i)
    result = zisc::isNan(get(i));
  return result;
}

/*!
  */
bool SpectralDistribution::hasNegative() const noexcept
{
  bool result = false;
  for (uint i = 0; (i < size()) && !result; ++i)
    result = zisc::isNegative(get(i));
  return result;
}

/*!
  */
bool SpectralDistribution::hasValue(const Float value) const noexcept
{
  bool result = false;
  for (uint i = 0; (i < size()) && !result; ++i)
    result = (get(i) == value);
  return result;
}

/*!
  */
bool SpectralDistribution::isAllInBounds(const Float lower,
                                         const Float upper) const noexcept
{
  bool result = true;
  for (uint i = 0; (i < size()) && result; ++i)
    result = zisc::isInBounds(get(i), lower, upper);
  return result;
}

/*!
  */
bool SpectralDistribution::isAllInClosedBounds(const Float lower,
                                               const Float upper) const noexcept
{
  bool result = true;
  for (uint i = 0; (i < size()) && result; ++i)
    result = zisc::isInClosedBounds(get(i), lower, upper);
  return result;
}

/*!
  */
bool SpectralDistribution::isAllZero() const noexcept
{
  constexpr Float zero = zisc::cast<Float>(0.0);
  bool result = true;
  for (uint i = 0; (i < size()) && result; ++i)
    result = (get(i) == zero);
  return result;
}

/*!
  */
auto SpectralDistribution::makeDistribution(const RepresentationType type,
                                            zisc::pmr::memory_resource* mem_resource,
                                            const bool is_compensated) noexcept
    -> SpectralDistributionPointer
{
  SpectralDistributionPointer distribution;
  switch (type) {
   case RepresentationType::kRgb: {
    if (is_compensated) {
      distribution = zisc::UniqueMemoryPointer<CompensatedRgbDistribution>::make(
          mem_resource);
    }
    else {
      distribution = zisc::UniqueMemoryPointer<RgbDistribution>::make(
          mem_resource);
    }
    break;
   }
   case RepresentationType::kSpectra: {
    if (is_compensated) {
      distribution = zisc::UniqueMemoryPointer<CompensatedSpectraDistribution>::make(
          mem_resource);
    }
    else {
      distribution = zisc::UniqueMemoryPointer<SpectraDistribution>::make(
          mem_resource);
    }
    break;
   }
   default: {
    zisc::raiseError("SpectralDistributionError: Unsupported representation type.");
    break;
   }
  }
  return distribution;
}

/*!
  */
auto SpectralDistribution::makeDistribution(const System& system,
                                            const SettingNodeBase* settings,
                                            zisc::pmr::memory_resource* mem_resource,
                                            const bool is_compensated) noexcept
    -> SpectralDistributionPointer
{
  const auto spectra_settings = castNode<SpectraSettingNode>(settings);
  auto distribution = makeDistribution(spectra_settings->representationType(),
                                       mem_resource,
                                       is_compensated);
  distribution->load(system, settings);
  return distribution;
}

/*!
  */
Float SpectralDistribution::max() const noexcept
{
  Float max_value = get(0);
  for (uint i = 1; i < size(); ++i)
    max_value = zisc::max(max_value, get(i));
  return max_value;
}

/*!
  */
Float SpectralDistribution::min() const noexcept
{
  Float min_value = get(0);
  for (uint i = 1; i < size(); ++i)
    min_value = zisc::min(min_value, get(i));
  return min_value;
}

/*!
  */
void SpectralDistribution::normalize() noexcept
{
  ZISC_ASSERT(!isAllZero(), "The intensities are all zero.");
  const Float k = zisc::invert(compensatedSum());
  for (uint i = 0; i < size(); ++i) {
    const Float v = k * get(i);
    set(i, v);
  }
}

/*!
  */
Float SpectralDistribution::sum() const noexcept
{
  Float sum = zisc::cast<Float>(0.0);
  for (uint i = 0; i < size(); ++i)
    sum += get(i);
  return sum;
}

/*!
  */
Float SpectralDistribution::compensatedSum() const noexcept
{
  zisc::CompensatedSummation<Float> sum;
  for (uint i = 0; i < size(); ++i)
    sum.add(get(i));
  return sum.get();
}

} // namespace nanairo
