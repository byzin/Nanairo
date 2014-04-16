/*!
  \file ggx_dielectric_bsdf.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_GGX_DIELECTRIC_BSDF_HPP_
#define _NANAIRO_GGX_DIELECTRIC_BSDF_HPP_

// Standard C++ library
#include <tuple>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
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
class GgxDielectricBsdf : public GlossyShaderModel<kSampleSize>
{
 public:
  using Spectra = typename ShaderModel<kSampleSize>::Spectra;
  using Wavelengths = typename ShaderModel<kSampleSize>::Wavelengths;


  //! Create a GGX dielectric BSDF
  GgxDielectricBsdf(const Float roughness, const Float n);


  //! Evaluate the pdf
  Float evaluatePdf(const Vector3* vin,
                    const Vector3* vout,
                    const Vector3& normal,
                    const Wavelengths& wavelengths) const override;

  //! Evaluate the radiance of the area sampling
  Spectra evaluateRadiance(const Vector3* vin,
                           const Vector3* vout,
                           const Vector3& normal,
                           const Wavelengths& wavelengths) const override;

  //! Evaluate the radiance of the area sampling
  std::tuple<Spectra, Float> evaluateRadianceAndPdf(
      const Vector3* vin,
      const Vector3* vout,
      const Vector3& normal,
      const Wavelengths& wavelengths) const override;

  //! Sample a reflection direction and evaluate a reflection weight
  std::tuple<SampledDirection, Spectra> sample(
      const Vector3* vin,
      const Vector3& normal,
      const Wavelengths& wavelengths,
      Sampler& sampler) const override;

  //! Check if wavelength selection occured
  bool wavelengthIsSelected() const override;

 private:
  const Float roughness_;
  const Float n_;
};

//! \} Core

} // namespace nanairo

#include "ggx_dielectric_bsdf-inl.hpp"

#endif // _NANAIRO_GGX_DIELECTRIC_BSDF_HPP_
