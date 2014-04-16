/*!
  \file specular_brdf.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SPECULAR_BRDF_HPP_
#define _NANAIRO_SPECULAR_BRDF_HPP_

// Standard C++ library
#include <tuple>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
class Sampler;
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class SpecularBrdf : public SpecularShaderModel<kSampleSize>
{
 public:
  using Spectra = typename ShaderModel<kSampleSize>::Spectra;
  using Wavelengths = typename ShaderModel<kSampleSize>::Wavelengths;


  //! Create a specular BRDF
  SpecularBrdf(const Spectra& reflectance_0deg);


  //! Evalute the weight of solid angle sampling
  std::tuple<SampledDirection, Spectra> sample(
      const Vector3* vin,
      const Vector3& normal,
      const Wavelengths& wavelengths,
      Sampler& sampler) const override;

  //! Check if wavelength selection occured
  bool wavelengthIsSelected() const override;

 private:
  const Spectra reflectance_0deg_;
};

//! \} Core

} // namespace nanairo

#include "specular_brdf-inl.hpp"

#endif // _NANAIRO_SPECULAR_BRDF_HPP_
