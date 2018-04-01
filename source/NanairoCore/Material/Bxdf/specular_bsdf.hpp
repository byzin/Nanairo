/*!
  \file specular_bsdf.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECULAR_BSDF_HPP
#define NANAIRO_SPECULAR_BSDF_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class Sampler;
class SampledSampledSpectra;
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class SpecularBsdf : public SpecularShaderModel
{
 public:
  //! Create a specular BSDF
  SpecularBsdf(const Float n) noexcept;


  //! Check if the BSDF is reflective
  bool isReflective() const noexcept override;

  //! Check if the BSDF is transmissive
  bool isTransmissive() const noexcept override;

  //! Evalute the weight of solid angle sampling
  std::tuple<SampledDirection, SampledSpectra> sample(
      const Vector3* vin,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      const IntersectionInfo* info) const noexcept override;

  //! Check if wavelength selection occured
  bool wavelengthIsSelected() const noexcept override;

 private:
  Float n_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SPECULAR_BSDF_HPP
