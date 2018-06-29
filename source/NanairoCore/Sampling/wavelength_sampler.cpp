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
#include "sampled_wavelengths.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/path_state.hpp"
#include "NanairoCore/Setting/system_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Utility/value.hpp"
#include "Sampler/sampler.hpp"

namespace nanairo {

/*!
  */
WavelengthSampler::WavelengthSampler(const World& world,
                                     const SettingNodeBase* settings) noexcept
{
  initialize(world, settings);
}

/*!
  */
void WavelengthSampler::initialize(const World& /* world */,
                                   const SettingNodeBase* settings) noexcept
{
  ZISC_ASSERT(settings != nullptr, "The setting node is null.");
  ZISC_ASSERT(settings->type() == SettingNodeType::kSystem,
              "Wrong setting node is specified.");
  const auto system_settings = zisc::cast<const SystemSettingNode*>(settings);

  if (system_settings->colorMode() == RenderingColorMode::kRgb) {
    type_ = SamplerType::kRgb;
  }
  else { // Spectra
    switch (system_settings->wavelengthSamplerType()) {
     case WavelengthSamplerType::kRegular: {
      type_ = SamplerType::kRegular;
      break;
     }
     case WavelengthSamplerType::kRandom: {
       type_ = SamplerType::kRandom;
       break;
     }
     case WavelengthSamplerType::kStratified: {
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
SampledWavelengths WavelengthSampler::sampleRgb(
    Sampler& sampler,
    PathState& path_state) noexcept
{
  constexpr uint sample_size = SampledWavelengths::size();
  path_state.setDimension(path_state.dimension() + sample_size);

  SampledWavelengths sampled_wavelengths;
  sampled_wavelengths.set(0, CoreConfig::blueWavelength(), 1.0);
  sampled_wavelengths.set(1, CoreConfig::greenWavelength(), 1.0);
  sampled_wavelengths.set(2, CoreConfig::redWavelength(), 1.0);
  sampled_wavelengths.selectPrimaryWavelength(sampler, path_state);
  return sampled_wavelengths;

}

/*!
  \details
  No detailed.
  */
SampledWavelengths WavelengthSampler::sampleRandomly(
    Sampler& sampler,
    PathState& path_state) noexcept
{
  using zisc::cast;
  constexpr uint sample_size = SampledWavelengths::size();
  constexpr Float inverse_probability = cast<Float>(CoreConfig::spectraSize()) /
                                        cast<Float>(sample_size);

  std::array<uint16, sample_size> wavelengths;
  for (uint i = 0; i < sample_size; ++i) {
    const Float offset = sampler.draw1D(path_state);
    path_state.setDimension(path_state.dimension() + 1);
    const Float position = cast<Float>(CoreConfig::spectraSize()) * offset;
    const uint index = cast<uint>(position);
    const uint16 wavelength = getWavelength(index);
    wavelengths[i] = wavelength;
  }
  std::sort(wavelengths.begin(), wavelengths.end());

  SampledWavelengths sampled_wavelengths;
  for (uint i = 0; i < sample_size; ++i)
    sampled_wavelengths.set(i, wavelengths[i], inverse_probability);
  sampled_wavelengths.selectPrimaryWavelength(sampler, path_state);
  return sampled_wavelengths;
}

/*!
  \details
  No detailed.
  */
SampledWavelengths WavelengthSampler::sampleRegularly(
    Sampler& sampler,
    PathState& path_state) noexcept
{
  using zisc::cast;
  constexpr uint sample_size = SampledWavelengths::size();
  constexpr Float interval = cast<Float>(CoreConfig::spectraSize()) /
                             cast<Float>(sample_size);
  constexpr Float inverse_probability = interval;

  SampledWavelengths sampled_wavelengths;
  const Float offset = sampler.draw1D(path_state);
  for (uint i = 0; i < sample_size; ++i) {
    path_state.setDimension(path_state.dimension() + 1);
    const uint index = cast<uint>(interval * (cast<Float>(i) + offset));
    const uint16 wavelength = getWavelength(index);
    sampled_wavelengths.set(i, wavelength, inverse_probability);
  }
  sampled_wavelengths.selectPrimaryWavelength(sampler, path_state);
  return sampled_wavelengths;
}

/*!
  */
SampledWavelengths WavelengthSampler::sampleStratified(
    Sampler& sampler,
    PathState& path_state) noexcept
{
  using zisc::cast;
  constexpr uint sample_size = SampledWavelengths::size();
  constexpr Float interval = cast<Float>(CoreConfig::spectraSize()) /
                             cast<Float>(sample_size);
  constexpr Float inverse_probability = interval;

  SampledWavelengths sampled_wavelengths;
  for (uint i = 0; i < sample_size; ++i) {
    const Float offset = sampler.draw1D(path_state);
    path_state.setDimension(path_state.dimension() + 1);
    const Float position = interval * (cast<Float>(i) + offset);
    const uint index = cast<uint>(position);
    const uint16 wavelength = getWavelength(index);
    sampled_wavelengths.set(i, wavelength, inverse_probability);
  }
  sampled_wavelengths.selectPrimaryWavelength(sampler, path_state);
  return sampled_wavelengths;
}

} // namespace nanairo
