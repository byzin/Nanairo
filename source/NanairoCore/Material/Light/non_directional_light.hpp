/*!
  \file non_directional_light.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_NON_DIRECTIONAL_LIGHT_HPP
#define NANAIRO_NON_DIRECTIONAL_LIGHT_HPP

// Standard C++ library
#include <tuple>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class SampledDirection;
class Sampler;
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class NonDirectionalLight : public DiffuseShaderModel
{
 public:
  //! Create a non-directional light
  NonDirectionalLight(const SampledSpectra& radiant_exitance) noexcept;


  //! Evaluate the radiance of area sampling
  SampledSpectra evalRadiance(
      const Vector3* vin,
      const Vector3* vout,
      const WavelengthSamples& wavelengths,
      const IntersectionInfo* info) const noexcept override;

  //! Sample a light ray direction and evaluate a ray weight
  std::tuple<SampledDirection, SampledSpectra> sample(
      const Vector3* vin,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      const IntersectionInfo* info) const noexcept override;

  //! Check is wavelength selection occured
  bool wavelengthIsSelected() const noexcept override;

 private:
  const SampledSpectra radiant_exitance_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_NON_DIRECTIONAL_LIGHT_HPP
