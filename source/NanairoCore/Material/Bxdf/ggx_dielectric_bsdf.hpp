/*!
  \file ggx_dielectric_bsdf.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_GGX_DIELECTRIC_BSDF_HPP
#define NANAIRO_GGX_DIELECTRIC_BSDF_HPP

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
class GgxDielectricBsdf : public GlossyShaderModel
{
 public:
  //! Create a GGX dielectric BSDF
  GgxDielectricBsdf(const Float roughness, const Float n) noexcept;


  //! Evaluate the pdf
  Float evalPdf(const Vector3* vin,
                const Vector3* vout,
                const Vector3& normal,
                const WavelengthSamples& wavelengths) const noexcept override;

  //! Evaluate the radiance of the area sampling
  SampledSpectra evalRadiance(
      const Vector3* vin,
      const Vector3* vout,
      const Vector3& normal,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Evaluate the radiance of the area sampling
  std::tuple<SampledSpectra, Float> evalRadianceAndPdf(
      const Vector3* vin,
      const Vector3* vout,
      const Vector3& normal,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Sample a reflection direction and evaluate a reflection weight
  std::tuple<SampledDirection, SampledSpectra> sample(
      const Vector3* vin,
      const Vector3& normal,
      const WavelengthSamples& wavelengths,
      Sampler& sampler) const noexcept override;

  //! Check if wavelength selection occured
  bool wavelengthIsSelected() const noexcept override;

 private:
  const Float roughness_;
  const Float n_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_GGX_DIELECTRIC_BSDF_HPP
