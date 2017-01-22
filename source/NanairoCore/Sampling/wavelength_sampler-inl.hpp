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
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  */
inline
SampledWavelengths WavelengthSampler::operator()(Sampler& sampler) const noexcept
{
  return sample(sampler);
}

/*!
  */
inline
SampledWavelengths WavelengthSampler::sample(Sampler& sampler) const noexcept
{
  return (type_ == SamplerType::kRgb)     ? sampleRgb(sampler) :
         (type_ == SamplerType::kRegular) ? sampleRegularly(sampler) :
         (type_ == SamplerType::kRandom)  ? sampleRandomly(sampler)
                                          : sampleStratified(sampler);
}

} // namespace nanairo

#endif // NANAIRO_WAVELENGTH_SAMPLER_INL_HPP
