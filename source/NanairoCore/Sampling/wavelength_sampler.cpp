/*!
  \file wavelength_sampler.cpp
  \author Sho Ikeda
  */

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
WavelengthSampler::WavelengthSampler(const World& world,
                                     const QJsonObject& settings) noexcept
{
  initialize(world, settings);
}

/*!
  */
void WavelengthSampler::initialize(const World& /* world */,
                                   const QJsonObject& settings) noexcept
{
  using zisc::toHash32;
  const auto mode = SceneValue::toString(settings, keyword::colorMode);
  if (mode == keyword::rgb) { // RGB
    type_ = SamplerType::kRgb;
  }
  else { // Spectra
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
}

/*!
  */
SampledWavelengths WavelengthSampler::sampleRgb(Sampler& sampler) noexcept
{
  SampledWavelengths sampled_wavelengths;
  sampled_wavelengths.set(0, CoreConfig::blueWavelength(), 1.0);
  sampled_wavelengths.set(1, CoreConfig::greenWavelength(), 1.0);
  sampled_wavelengths.set(2, CoreConfig::redWavelength(), 1.0);
  sampled_wavelengths.selectPrimaryWavelength(sampler);
  return sampled_wavelengths;

}

/*!
  \details
  No detailed.
  */
SampledWavelengths WavelengthSampler::sampleRandomly(Sampler& sampler) noexcept
{
  using zisc::cast;
  constexpr uint sample_size = SampledWavelengths::size();
  constexpr Float inverse_probability = cast<Float>(CoreConfig::spectraSize()) /
                                        cast<Float>(sample_size);

  std::array<uint16, sample_size> wavelengths;
  for (uint i = 0; i < sample_size; ++i) {
    const Float position = cast<Float>(CoreConfig::spectraSize()) *
                           sampler.sample();
    const uint index = cast<uint>(position);
    const uint16 wavelength = getWavelength(index);
    wavelengths[i] = wavelength;
  }
  std::sort(wavelengths.begin(), wavelengths.end());

  SampledWavelengths sampled_wavelengths;
  for (uint i = 0; i < sample_size; ++i)
    sampled_wavelengths.set(i, wavelengths[i], inverse_probability);
  sampled_wavelengths.selectPrimaryWavelength(sampler);
  return sampled_wavelengths;
}

/*!
  \details
  No detailed.
  */
SampledWavelengths WavelengthSampler::sampleRegularly(Sampler& sampler) noexcept
{
  using zisc::cast;
  constexpr uint sample_size = SampledWavelengths::size();
  constexpr Float interval = cast<Float>(CoreConfig::spectraSize()) /
                             cast<Float>(sample_size);
  constexpr Float inverse_probability = interval;

  SampledWavelengths sampled_wavelengths;
  const Float offset = sampler.sample();
  for (uint i = 0; i < sample_size; ++i) {
    const uint index = cast<uint>(interval * (cast<Float>(i) + offset));
    const uint16 wavelength = getWavelength(index);
    sampled_wavelengths.set(i, wavelength, inverse_probability);
  }
  sampled_wavelengths.selectPrimaryWavelength(sampler);
  return sampled_wavelengths;
}

/*!
  */
SampledWavelengths WavelengthSampler::sampleStratified(Sampler& sampler) noexcept
{
  using zisc::cast;
  constexpr uint sample_size = SampledWavelengths::size();
  constexpr Float interval = cast<Float>(CoreConfig::spectraSize()) /
                             cast<Float>(sample_size);
  constexpr Float inverse_probability = interval;

  SampledWavelengths sampled_wavelengths;
  for (uint i = 0; i < sample_size; ++i) {
    const Float position = interval * (cast<Float>(i) + sampler.sample());
    const uint index = cast<uint>(position);
    const uint16 wavelength = getWavelength(index);
    sampled_wavelengths.set(i, wavelength, inverse_probability);
  }
  sampled_wavelengths.selectPrimaryWavelength(sampler);
  return sampled_wavelengths;
}

} // namespace nanairo
