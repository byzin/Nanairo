/*!
  \file sampled_wavelengths-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLED_WAVELENGTHS_INL_HPP
#define NANAIRO_SAMPLED_WAVELENGTHS_INL_HPP

#include "sampled_wavelengths.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

// Forward declaration
class World;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledWavelengths<kSampleSize>::SampledWavelengths() noexcept
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto SampledWavelengths<kSampleSize>::inverseProbabilities() const noexcept
    -> const Intensities&
{
  return inverse_probabilities_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> template <uint kMax> inline
void SampledWavelengths<kSampleSize>::selectPrimaryWavelength(Sampler& sampler) noexcept
{
  constexpr uint min = 0;
  const uint selected_index = sampler.sample(min, kMax);
  wavelengths_.setPrimaryWavelength(selected_index);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void SampledWavelengths<kSampleSize>::set(const uint index, 
                                          const uint16 wavelength,
                                          const Float inverse_probability) noexcept
{
  wavelengths_[index] = wavelength;
  inverse_probabilities_[index] = inverse_probability;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto SampledWavelengths<kSampleSize>::wavelengths() const noexcept -> const Wavelengths&
{
  return wavelengths_;
}

/*!
  \details
  No detailed.
  */
//template <uint kSampleSize> inline
//WavelengthSampler<kSampleSize> makeLightsBasedImportanceSampler(const World& world)
//{
//  const auto pdf = calculateLightPdf(world);
//  SpectralDistribution inverse_pdf;
//  for (uint i = 0; i < pdf.size(); ++i)
//    inverse_pdf[i] = 1.0 / pdf[i];
//  CumulativeDistributionFunction cdf{pdf};
//
//  return makeImportanceSampler<kSampleSize>(inverse_pdf, cdf);
//}

/*!
  \details
  No detailed.
  */
//template <uint kSampleSize> inline
//WavelengthSampler<kSampleSize> makeImportanceSampler(
//    const SpectralDistribution& inverse_pdf,
//    const CumulativeDistributionFunction& cdf)
//{
//  using zisc::cast;
//
//  WavelengthSampler<kSampleSize> wavelength_sampler{
//  [inverse_pdf, cdf](Sampler& sampler)
//  {
//    constexpr Float k = 1.0 / cast<Float>(kSampleSize);
//    const Float offset = sampler.sample(0.0, 1.0);
//    SampledWavelengths<kSampleSize> sampled_wavelengths;
//    for (uint i = 0; i < kSampleSize; ++i) {
//      const Float y = (cast<Float>(i) + offset) * k;
//      const auto index = cdf.inverseFunction(y);
//      const uint16 wavelength = getWavelength(index);
//      const Float inverse_probability = inverse_pdf.get(index);
//      sampled_wavelengths.set(i, wavelength, inverse_probability);
//    }
//    sampled_wavelengths.template selectPrimaryWavelength<kSampleSize>(sampler);
//    return sampled_wavelengths;
//  }};
//  return wavelength_sampler;
//}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
WavelengthSampler<kSampleSize> makeRgbSampler() noexcept
{
  WavelengthSampler<kSampleSize> wavelength_sampler{[](Sampler& sampler)
  {
    return sampleRgbWavelengths<kSampleSize>(sampler);
  }};
  return wavelength_sampler;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
WavelengthSampler<kSampleSize> makeWavelengthSampler(
    const World& /* world */,
    const QJsonObject& settings) noexcept
{
  using zisc::toHash32;

  WavelengthSampler<kSampleSize> wavelength_sampler;
  const auto method = stringValue(settings, keyword::wavelengthSampling);
  switch (keyword::toHash32(method)) {
    case toHash32(keyword::regularSampling): {
      wavelength_sampler = sampleWavelengthsRegularly<kSampleSize>;
      break;
    }
    case toHash32(keyword::randomSampling): {
      wavelength_sampler = sampleWavelengthsRandomly<kSampleSize>;
      break;
    }
    case toHash32(keyword::stratifiedSampling): {
      wavelength_sampler = sampleStratifiedWavelengths<kSampleSize>;
      break;
    }
//   case toHash32(keyword::lightsBasedImportanceSampling):
//    wavelength_sampler = makeLightsBasedImportanceSampler<kSampleSize>(world);
//    break;
    default: {
      zisc::raiseError("SystemError: Unsupported wavelength sampler is specified.");
      break;
    }
  }
  return wavelength_sampler;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledWavelengths<kSampleSize> sampleRgbWavelengths(Sampler& sampler) noexcept
{
  SampledWavelengths<kSampleSize> sampled_wavelengths;
  sampled_wavelengths.set(0, kBlueWavelength, 1.0);
  sampled_wavelengths.set(1, kGreenWavelength, 1.0);
  sampled_wavelengths.set(2, kRedWavelength, 1.0);

  sampled_wavelengths.template selectPrimaryWavelength<kSampleSize>(sampler);
  return sampled_wavelengths;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledWavelengths<kSampleSize> sampleStratifiedWavelengths(Sampler& sampler) noexcept
{
  using zisc::cast;

  constexpr Float interval = cast<Float>(kSpectraSize) / cast<Float>(kSampleSize);
  constexpr Float inverse_probability = interval;

  SampledWavelengths<kSampleSize> sampled_wavelengths;
  for (uint i = 0; i < SampledWavelengths<kSampleSize>::size(); ++i) {
    const Float position = interval * (cast<Float>(i) + sampler.sample(0.0, 1.0));
    const uint index = cast<uint>(position);
    const uint16 wavelength = getWavelength(index);
    sampled_wavelengths.set(i, wavelength, inverse_probability);
  }
  sampled_wavelengths.template selectPrimaryWavelength<kSampleSize>(sampler);
  return sampled_wavelengths;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledWavelengths<kSampleSize> sampleWavelengthsRandomly(Sampler& sampler) noexcept
{
  using zisc::cast;

  constexpr Float inverse_probability = cast<Float>(kSpectraSize) / 
                                        cast<Float>(kSampleSize);

  std::array<uint16, kSampleSize> wavelengths;
  for (uint i = 0; i < SampledWavelengths<kSampleSize>::size(); ++i) {
    const Float position = cast<Float>(kSpectraSize) * sampler.sample(0.0, 1.0);
    const uint index = cast<uint>(position);
    const uint16 wavelength = getWavelength(index);
    wavelengths[i] = wavelength;
  }
  std::sort(wavelengths.begin(), wavelengths.end());

  SampledWavelengths<kSampleSize> sampled_wavelengths;
  for (uint i = 0; i < SampledWavelengths<kSampleSize>::size(); ++i)
    sampled_wavelengths.set(i, wavelengths[i], inverse_probability);
  sampled_wavelengths.template selectPrimaryWavelength<kSampleSize>(sampler);
  return sampled_wavelengths;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledWavelengths<kSampleSize> sampleWavelengthsRegularly(Sampler& sampler) noexcept
{
  using zisc::cast;

  constexpr Float interval = cast<Float>(kSpectraSize) / cast<Float>(kSampleSize);
  constexpr Float inverse_probability = interval;

  SampledWavelengths<kSampleSize> sampled_wavelengths;
  const Float offset = sampler.sample(0.0, 1.0);
  for (uint i = 0; i < SampledWavelengths<kSampleSize>::size(); ++i) {
    const uint index = cast<uint>(interval * (cast<Float>(i) + offset));
    const uint16 wavelength = getWavelength(index);
    sampled_wavelengths.set(i, wavelength, inverse_probability);
  }
  sampled_wavelengths.template selectPrimaryWavelength<kSampleSize>(sampler);
  return sampled_wavelengths;
}

} // namespace nanairo

#endif // NANAIRO_SAMPLED_WAVELENGTHS_INL_HPP
