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
  //! The types of GGX method
  enum class GgxMethodType : int
  {
    kSmith,
    kVCavity
  };


  //! Evaluate the D
  static Float evalD(const Float roughness, const Float cos_nm) noexcept;

  //! Evaluate the G1 term
  static Float evalG1(const Float roughness,
                      const Float cos_n,
                      const Float cos_m,
                      const Float cos_nm) noexcept;

  //! Evaluate the G1 term
  static Float evalG2(const Float roughness,
                      const Float cos_ni,
                      const Float cos_no,
                      const Float cos_mi,
                      const Float cos_mo,
                      const Float cos_nm) noexcept;

  //! Evaluate the GGX reflectance of a dielectric material
  static Float evalReflectance(const Float roughness,
                               const Vector3& vin,
                               const Vector3& vout,
                               const Vector3& normal,
                               const Float n,
                               Float* pdf = nullptr) noexcept;

  //! Evaluate the GGX reflectance
  static SampledSpectra evalReflectance(
      const Float roughness,
      const Vector3& vin,
      const Vector3& vout,
      const Vector3& normal,
      const SampledSpectra& n,
      const SampledSpectra& eta,
      Float* pdf = nullptr) noexcept;

  //! Evaluate the reflection pdf for the conductor
  static Float evalReflectionPdf(const Float roughness,
                                 const Vector3& vin,
                                 const Vector3& vout,
                                 const Vector3& normal) noexcept;

  //! Evaluate the refraction pdf fo the dielectric
  static Float evalRefractionPdf(const Float roughness,
                                 const Vector3& vin,
                                 const Vector3& vout,
                                 const Vector3& normal,
                                 const Float n) noexcept;

  //! Evaluate the GGX transmittance of a dielectric material
  static Float evalTransmittance(const Float roughness,
                                 const Vector3& vin,
                                 const Vector3& vout,
                                 const Vector3& normal,
                                 const Float n,
                                 Float* pdf = nullptr) noexcept;

  //! Evaluate the weight
  static Float evalWeight(const Float roughness,
                          const Float cos_ni,
                          const Float cos_no,
                          const Float cos_mi,
                          const Float cos_mo,
                          const Float cos_nm) noexcept;

  //! Sample a GGX microfacet normal
  static SampledDirection sampleNormal(const Float roughness,
                                       const Vector3& vin,
                                       const Vector3& normal,
                                       Sampler& sampler,
                                       const bool calc_pdf = true) noexcept;


//  static constexpr GgxMethodType kUsedType = GgxMethodType::kSmith;
  static constexpr GgxMethodType kUsedType = GgxMethodType::kVCavity;

 private:
  template <GgxMethodType kMethod>
  class GgxMethod
  {
   public:
    //! Evaluate the G1 term
    static Float evalG1(const Float roughness,
                        const Float cos_n,
                        const Float cos_m,
                        const Float cos_nm) noexcept;

    //! Evaluate the G1 term
    static Float evalG2(const Float roughness,
                        const Float cos_ni,
                        const Float cos_no,
                        const Float cos_mi,
                        const Float cos_mo,
                        const Float cos_nm) noexcept;

    //! Evaluate the weight
    static Float evalWeight(const Float roughness,
                            const Float cos_ni,
                            const Float cos_no,
                            const Float cos_mi,
                            const Float cos_mo,
                            const Float cos_nm) noexcept;

    //! Sample the microfacet normal
    static Vector3 sampleMicrofacetNormal(const Float roughness,
                                          const Vector3& vin,
                                          Sampler& sampler) noexcept;

   private:
    //! Sample the slope XY values
    static std::tuple<Float, Float> smithSampleSlopeXY(const Float cos_theta,
                                                       Sampler& sampler) noexcept;

    //! Stretch the microfacet normal
    static Vector3 smithStretch(const Float roughness,
                                const Vector3& vin) noexcept;

    //! Sample a microfacet normal
    static Vector3 vcavitySampleMicrofacetNormal(const Float roughness,
                                                 Sampler& sampler) noexcept;
  };

  //! Calculate the pdf of the GGX reflection
  static Float calcReflectionPdf(const Float roughness,
                                 const Float d,
                                 const Float cos_ni,
                                 const Float cos_mi,
                                 const Float cos_nm) noexcept;

  //! Calculate the pdf of the GGX refraction
  static Float calcRefractionPdf(const Float roughness,
                                 const Float d,
                                 const Float cos_ni,
                                 const Float cos_mi,
                                 const Float cos_mo,
                                 const Float cos_nm,
                                 const Float n) noexcept;

  //! Sample the microfacet normal
  static Vector3 sampleMicrofacetNormal(const Float roughness,
                                        const Vector3& vin,
                                        Sampler& sampler) noexcept;


  // Methods
  using Smith = GgxMethod<GgxMethodType::kSmith>;
  using VCavity = GgxMethod<GgxMethodType::kVCavity>;
};

//! \} Core

} // namespace nanairo

#include "microfacet_ggx-inl.hpp"

#endif // NANAIRO_MICROFACET_GGX_HPP
