/*!
  \file non_directional_light.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_NON_DIRECTIONAL_LIGHT_HPP_
#define _NANAIRO_NON_DIRECTIONAL_LIGHT_HPP_

// Standard C++ library
#include <tuple>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
class SampledDirection;
class Sampler;
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class NonDirectionalLight : public DiffuseShaderModel<kSampleSize>
{
 public:
  using Spectra = typename ShaderModel<kSampleSize>::Spectra;
  using Wavelengths = typename ShaderModel<kSampleSize>::Wavelengths;

  //! Create a non-directional light 
  NonDirectionalLight(const Spectra& radiant_exitance);
  

  //! Evaluate the radiance of area sampling
  Spectra evaluateRadiance(const Vector3* vin,
                           const Vector3* vout,
                           const Vector3& normal,
                           const Wavelengths& wavelengths) const override;

  //! Sample a light ray direction and evaluate a ray weight
  std::tuple<SampledDirection, Spectra> sample(
      const Vector3* vin,
      const Vector3& normal,
      const Wavelengths& wavelengths,
      Sampler& sampler) const override;

  //! Check is wavelength selection occured
  bool wavelengthIsSelected() const override;

 private:
  const Spectra radiant_exitance_;
};

//! \} Core

} // namespace nanairo

#include "non_directional_light-inl.hpp"

#endif // _NANAIRO_NON_DIRECTIONAL_LIGHT_HPP_
