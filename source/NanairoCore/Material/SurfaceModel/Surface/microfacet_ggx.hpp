/*!
  \file microfacet_ggx.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MICROFACET_GGX_HPP
#define NANAIRO_MICROFACET_GGX_HPP

// Standard C++ library
#include <tuple>
#include <utility>
// Nanairo
#include "microfacet.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  */
class MicrofacetGgx : public Microfacet
{
 public:
  //! Evaluate the D
  static Float evalD(const Float roughness_x,
                     const Float roughness_y, 
                     const Vector3& m_normal) noexcept;

  //! Evaluate the G1 term
  static Float evalG1(const Float roughness_x,
                      const Float roughness_y,
                      const Vector3& v,
                      const Vector3& m_normal) noexcept;

  //! Evaluate the G2 term
  static Float evalG2(const Float roughness_x,
                      const Float roughness_y,
                      const Vector3& vin,
                      const Vector3& vout,
                      const Vector3& m_normal) noexcept;

  //! Evaluate the refraction pdf fo the dielectric
  static Float evalRefractionPdf(const Float roughness_x,
                                 const Float roughness_y,
                                 const Vector3& vin,
                                 const Vector3& vout,
                                 const Vector3& m_normal,
                                 const Float n) noexcept;

  //! Evaluate the GGX reflectance of a dielectric material
  static Float evalReflectance(const Float roughness_x,
                               const Float roughness_y,
                               const Vector3& vin,
                               const Vector3& vout,
                               const Vector3& m_normal,
                               const Float n,
                               Float* pdf = nullptr) noexcept;

  //! Evaluate the GGX reflectance
  static SampledSpectra evalReflectance(const Float roughness_x,
                                        const Float roughness_y,
                                        const Vector3& vin,
                                        const Vector3& vout,
                                        const Vector3& m_normal,
                                        const SampledSpectra& n,
                                        const SampledSpectra& eta,
                                        Float* pdf = nullptr) noexcept;

  //! Evaluate the refraction pdf fo the dielectric
  static Float evalReflectionPdf(const Float roughness_x,
                                 const Float roughness_y,
                                 const Vector3& vin,
                                 const Vector3& m_normal) noexcept;

  //! Evaluate the GGX transmittance of a dielectric material
  static Float evalTransmittance(const Float roughness_x,
                                 const Float roughness_y,
                                 const Vector3& vin,
                                 const Vector3& vout,
                                 const Vector3& m_normal,
                                 const Float n,
                                 Float* pdf = nullptr) noexcept;

  //! Evaluate the weight
  static Float evalWeight(const Float roughness_x,
                          const Float roughness_y,
                          const Vector3& vin,
                          const Vector3& vout,
                          const Vector3& m_normal) noexcept;

  //! Sample a GGX microfacet normal on the default basis
  static SampledDirection sampleNormal(const Float roughness_x,
                                       const Float roughness_y,
                                       const Vector3& vin,
                                       Sampler& sampler,
                                       const bool calc_pdf = true) noexcept;

 private:
  template <MicrosurfaceProfile kProfile>
  class GgxMicrosurface
  {
   public:
    //! Evaluate the weight
    static Float evalWeight(const Float roughness_x,
                            const Float roughness_y,
                            const Vector3& vin,
                            const Vector3& vout,
                            const Vector3& m_normal) noexcept;

    //! Sample a microfacet normal
    static Vector3 sampleMicrofacetNormal(const Float roughness_x,
                                          const Float roughness_y,
                                          const Vector3& vin,
                                          Sampler& sampler) noexcept;
  };

  //! Calculate the pdf of the GGX reflection
  static Float calcReflectionPdf(const Float roughness_x,
                                 const Float roughness_y,
                                 const Vector3& vin,
                                 const Vector3& m_normal,
                                 const Float d) noexcept;

  //! Calculate the pdf of the GGX refraction
  static Float calcRefractionPdf(const Float roughness_x,
                                 const Float roughness_y,
                                 const Vector3& vin,
                                 const Vector3& vout,
                                 const Vector3& m_normal,
                                 const Float n,
                                 const Float d) noexcept;

  //! Sample the microfacet normal
  static Vector3 sampleMicrofacetNormal(const Float roughness_x,
                                        const Float roughness_y,
                                        const Vector3& vin,
                                        Sampler& sampler) noexcept;


  // Microsurface profile of GGX
  static constexpr MicrosurfaceProfile kMicrosurface = MicrosurfaceProfile::kSmith;
//  static constexpr MicrosurfaceProfile kMicrosurface = MicrosurfaceProfile::kVCavity;
  using SmithGgxMicrosurface = GgxMicrosurface<MicrosurfaceProfile::kSmith>;
  using VCavityGgxMicrosurface = GgxMicrosurface<MicrosurfaceProfile::kVCavity>;
};

//! \} Core

} // namespace nanairo

#include "microfacet_ggx-inl.hpp"

#endif // NANAIRO_MICROFACET_GGX_HPP
