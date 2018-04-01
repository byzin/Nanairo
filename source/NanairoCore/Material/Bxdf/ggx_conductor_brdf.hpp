/*!
  \file ggx_conductor_brdf.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_GGX_CONDUCTOR_BRDF_HPP
#define NANAIRO_GGX_CONDUCTOR_BRDF_HPP

// Standard C++ library
#include <tuple>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
class Sampler;
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class GgxConductorBrdf : public GlossyShaderModel
{
 public:
  //! Create a GGX conductor BRDF
  GgxConductorBrdf(const Float roughness_x,
                   const Float roughness_y,
                   const SampledSpectra& n,
                   const SampledSpectra& eta) noexcept;


  //! Evaluate the pdf
  Float evalPdf(const Vector3* vin,
                const Vector3* vout,
                const WavelengthSamples& wavelengths,
                const IntersectionInfo* info) const noexcept override;

  //! Evaluate the radiance of the area sampling
  SampledSpectra evalRadiance(
      const Vector3* vin,
      const Vector3* vout,
      const WavelengthSamples& wavelengths,
      const IntersectionInfo* info) const noexcept override;

  //! Evaluate the radiance of the area sampling
  std::tuple<SampledSpectra, Float> evalRadianceAndPdf(
      const Vector3* vin,
      const Vector3* vout,
      const WavelengthSamples& wavelengths,
      const IntersectionInfo* info) const noexcept override;

  //! Check if the BRDF is reflective
  bool isReflective() const noexcept override;

  //! Sample a reflection direction and evaluate a reflection weight
  std::tuple<SampledDirection, SampledSpectra> sample(
      const Vector3* vin,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      const IntersectionInfo* info) const noexcept override;

  //! Check if wavelength selection occured
  bool wavelengthIsSelected() const noexcept override;

 private:
  const SampledSpectra n_,
                       eta_;
  const Float roughness_x_,
              roughness_y_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_GGX_CONDUCTOR_BRDF_HPP
