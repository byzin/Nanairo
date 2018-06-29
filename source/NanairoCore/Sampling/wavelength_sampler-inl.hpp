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
#include "sampled_wavelengths.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/value.hpp"
#include "Sampler/sampler.hpp"

namespace nanairo {

/*!
  */
inline
SampledWavelengths WavelengthSampler::operator()(
    Sampler& sampler,
    PathState& path_state) const noexcept
{
  return sample(sampler, path_state);
}

/*!
  */
inline
SampledWavelengths WavelengthSampler::sample(
    Sampler& sampler,
    PathState& path_state) const noexcept
{
  return (type_ == SamplerType::kRgb)     ? sampleRgb(sampler, path_state) :
         (type_ == SamplerType::kRegular) ? sampleRegularly(sampler, path_state) :
         (type_ == SamplerType::kRandom)  ? sampleRandomly(sampler, path_state)
                                          : sampleStratified(sampler, path_state);
}

} // namespace nanairo

#endif // NANAIRO_WAVELENGTH_SAMPLER_INL_HPP
