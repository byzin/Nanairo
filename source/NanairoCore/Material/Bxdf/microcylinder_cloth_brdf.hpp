/*!
  \file microcylinder_cloth_brdf.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MICROCYLINDER_CLOTH_BRDF_HPP
#define NANAIRO_MICROCYLINDER_CLOTH_BRDF_HPP

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
class ClothSurface;
class Sampler;
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class MicrocylinderClothBrdf : public GlossyShaderModel
{
 public:
  //! Create a GGX dielectric BSDF
  MicrocylinderClothBrdf(const ClothSurface* surface,
                         const SampledSpectra& reflectance) noexcept;


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
  SampledSpectra reflectance_;
  const ClothSurface* surface_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_MICROCYLINDER_CLOTH_BRDF_HPP
