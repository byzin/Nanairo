/*!
  \file specular_brdf.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECULAR_BRDF_HPP
#define NANAIRO_SPECULAR_BRDF_HPP

// Standard C++ library
#include <tuple>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class PathState;
class Sampler;
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class SpecularBrdf : public SpecularShaderModel
{
 public:
  //! Create a specular BRDF
  SpecularBrdf(const SampledSpectra& n, const SampledSpectra& eta) noexcept;


  //! Check if the BRDF is reflective
  bool isReflective() const noexcept override;

  //! Evalute the weight of solid angle sampling
  std::tuple<SampledDirection, SampledSpectra> sample(
      const Vector3* vin,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      PathState& path_state,
      const IntersectionInfo* info) const noexcept override;

  //! Check if wavelength selection occured
  bool wavelengthIsSelected() const noexcept override;

 private:
  const SampledSpectra n_,
                       eta_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SPECULAR_BRDF_HPP
