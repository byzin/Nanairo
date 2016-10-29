/*!
  \file wavelength_sampler-inl.hpp
  \author Sho Ikeda
  */

#ifndef NANAIRO_WAVELENGTH_SAMPLER_INL_HPP
#define NANAIRO_WAVELENGTH_SAMPLER_INL_HPP

#include "wavelength_sampler.hpp"
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "sampler.hpp"
#include "sampled_wavelengths.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/scene_value.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  */
template <uint kSampleSize> inline
WavelengthSampler<kSampleSize>::WavelengthSampler(const World& world,
                                                  const QJsonObject& settings) noexcept
{
  initialize(world, settings);
}

/*!
  */
template <uint kSampleSize> inline
SampledWavelengths<kSampleSize> WavelengthSampler<kSampleSize>::operator()(
    Sampler& sampler) const noexcept
{
  return sample(sampler);
}

/*!
  */
template <uint kSampleSize> inline
SampledWavelengths<kSampleSize> WavelengthSampler<kSampleSize>::sample(
    Sampler& sampler) const noexcept
{
  return (type_ == SamplerType::kRegular) ? sampleRegularly(sampler) :
         (type_ == SamplerType::kRandom)  ? sampleRandomly(sampler)
                                          : sampleStratified(sampler);
}

/*!
  */
template <> inline
SampledWavelengths<3> WavelengthSampler<3>::sample(
    Sampler& sampler) const noexcept
{
  SampledWavelengths<3> sampled_wavelengths;
  sampled_wavelengths.set(0, CoreConfig::blueWavelength(), 1.0);
  sampled_wavelengths.set(1, CoreConfig::greenWavelength(), 1.0);
  sampled_wavelengths.set(2, CoreConfig::redWavelength(), 1.0);

  sampled_wavelengths.selectPrimaryWavelength<3>(sampler);
  return sampled_wavelengths;

}

/*!
  */
template <uint kSampleSize> inline
void WavelengthSampler<kSampleSize>::initialize(const World& /* world */,
                                                const QJsonObject& settings) noexcept
{
  using zisc::toHash32;
  const auto method = SceneValue::toString(settings, keyword::wavelengthSampling);
  switch (keyword::toHash32(method)) {
   case toHash32(keyword::regularSampling): {
    type_ = SamplerType::kRegular;
    break;
   }
   case toHash32(keyword::randomSampling): {
     type_ = SamplerType::kRandom;
     break;
   }
   case toHash32(keyword::stratifiedSampling): {
     type_ = SamplerType::kStratified;
     break;
   }
   default: {
     zisc::raiseError("SamplingError: Unsupported wavelength sampler is specified.");
     break;
   }
  }
}

/*!
  */
template <> inline
void WavelengthSampler<3>::initialize(const World& /* world */,
                                      const QJsonObject& /* settings */) noexcept
{
  type_ = SamplerType::kRgb;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledWavelengths<kSampleSize> WavelengthSampler<kSampleSize>::sampleRandomly(
    Sampler& sampler) noexcept
{
  using zisc::cast;
  constexpr Float inverse_probability = cast<Float>(CoreConfig::spectraSize()) /
                                        cast<Float>(kSampleSize);

  std::array<uint16, kSampleSize> wavelengths;
  for (uint i = 0; i < kSampleSize; ++i) {
    const Float position = cast<Float>(CoreConfig::spectraSize()) *
                           sampler.sample(0.0, 1.0);
    const uint index = cast<uint>(position);
    const uint16 wavelength = getWavelength(index);
    wavelengths[i] = wavelength;
  }
  std::sort(wavelengths.begin(), wavelengths.end());

  SampledWavelengths<kSampleSize> sampled_wavelengths;
  for (uint i = 0; i < kSampleSize; ++i)
    sampled_wavelengths.set(i, wavelengths[i], inverse_probability);
  sampled_wavelengths.template selectPrimaryWavelength<kSampleSize>(sampler);
  return sampled_wavelengths;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledWavelengths<kSampleSize> WavelengthSampler<kSampleSize>::sampleRegularly(
    Sampler& sampler) noexcept
{
  using zisc::cast;
  constexpr Float interval = cast<Float>(CoreConfig::spectraSize()) /
                             cast<Float>(kSampleSize);
  constexpr Float inverse_probability = interval;

  SampledWavelengths<kSampleSize> sampled_wavelengths;
  const Float offset = sampler.sample(0.0, 1.0);
  for (uint i = 0; i < kSampleSize; ++i) {
    const uint index = cast<uint>(interval * (cast<Float>(i) + offset));
    const uint16 wavelength = getWavelength(index);
    sampled_wavelengths.set(i, wavelength, inverse_probability);
  }
  sampled_wavelengths.template selectPrimaryWavelength<kSampleSize>(sampler);
  return sampled_wavelengths;
}

/*!
  */
template <uint kSampleSize> inline
SampledWavelengths<kSampleSize> WavelengthSampler<kSampleSize>::sampleStratified(
    Sampler& sampler) noexcept
{
  using zisc::cast;
  constexpr Float interval = cast<Float>(CoreConfig::spectraSize()) /
                             cast<Float>(kSampleSize);
  constexpr Float inverse_probability = interval;

  SampledWavelengths<kSampleSize> sampled_wavelengths;
  for (uint i = 0; i < kSampleSize; ++i) {
    const Float position = interval * (cast<Float>(i) + sampler.sample(0.0, 1.0));
    const uint index = cast<uint>(position);
    const uint16 wavelength = getWavelength(index);
    sampled_wavelengths.set(i, wavelength, inverse_probability);
  }
  sampled_wavelengths.template selectPrimaryWavelength<kSampleSize>(sampler);
  return sampled_wavelengths;
}

} // namespace nanairo

#endif // NANAIRO_WAVELENGTH_SAMPLER_INL_HPP
