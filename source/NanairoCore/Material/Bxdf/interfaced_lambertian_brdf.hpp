/*!
  \file interfaced_lambertian_brdf.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_INTERFACED_LAMBERTIAN_BRDF_HPP
#define NANAIRO_INTERFACED_LAMBERTIAN_BRDF_HPP

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
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class InterfacedLambertianBrdf : public GlossyShaderModel<kSampleSize>
{
 public:
  using Spectra = typename ShaderModel<kSampleSize>::Spectra;
  using Wavelengths = typename ShaderModel<kSampleSize>::Wavelengths;


  //! Create a interfaced lambertian BRDF 
  InterfacedLambertianBrdf(const Float k_d,
                           const Float roughness,
                           const Float n,
                           const Float ri,
                           Sampler& sampler) noexcept;


  //! Evaluate the pdf
  Float evalPdf(const Vector3* vin,
                const Vector3* vout,
                const Vector3& normal,
                const Wavelengths& wavelengths) const noexcept override;

  //! Evaluate the radiance of the area sampling
  Spectra evalRadiance(const Vector3* vin,
                       const Vector3* vout,
                       const Vector3& normal,
                       const Wavelengths& wavelengths) const noexcept override;

  //! Evaluate the radiance of the area sampling
  std::tuple<Spectra, Float> evalRadianceAndPdf(
      const Vector3* vin,
      const Vector3* vout,
      const Vector3& normal,
      const Wavelengths& wavelengths) const noexcept override;

  //! Sample a reflection direction and evaluate a reflection weight
  std::tuple<SampledDirection, Spectra> sample(
      const Vector3* vin,
      const Vector3& normal,
      const Wavelengths& wavelengths,
      Sampler& sampler) const noexcept override;

  //! Check if wavelength selection occured
  bool wavelengthIsSelected() const noexcept override;

 private:
  Sampler* sampler_;
  const Float k_d_;
  const Float roughness_;
  const Float n_;
  const Float ri_;
};

//! \} Core

} // namespace nanairo

#include "interfaced_lambertian_brdf-inl.hpp"

#endif // NANAIRO_INTERFACED_LAMBERTIAN_BRDF_HPP
