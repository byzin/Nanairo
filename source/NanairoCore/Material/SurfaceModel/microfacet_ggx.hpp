/*!
  \file microfacet_ggx.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
  template <uint kSampleSize>
  static SampledSpectra<kSampleSize> evalReflectance(
      const Float roughness,
      const Vector3& vin,
      const Vector3& vout,
      const Vector3& normal,
      const SampledSpectra<kSampleSize>& r0,
      Float* pdf = nullptr) noexcept;

  //! Evaluate the reflection pdf for the conductor
  static Float evalReflectionPdf(const Float roughness,
                                 const Vector3& vin,
                                 const Vector3& vout,
                                 const Vector3& normal) noexcept;

  //! Evaluate the reflection pdf fo the dielectric
  static Float evalReflectionPdf(const Float roughness,
                                 const Vector3& vin,
                                 const Vector3& vout,
                                 const Vector3& normal,
                                 const Float n) noexcept;

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


  static constexpr GgxMethodType kUsedType = GgxMethodType::kSmith;

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


  // Methods
  using Smith = GgxMethod<GgxMethodType::kSmith>;
  using VCavity = GgxMethod<GgxMethodType::kVCavity>;
};

/*!
  */
class SampledGgxNormal
{
 public:
  //! Initialize a instance
  SampledGgxNormal(const SampledDirection& microfacet_normal,
                   const Float cos_ni,
                   const Float cos_mi,
                   const Float cos_nm) noexcept;


  //! Return the cos_ni
  Float cosNi() const noexcept;

  //! Return the cos_ni
  Float cosMi() const noexcept;

  //! Return the cos_ni
  Float cosNm() const noexcept;

  //! Return the microfacet normal
  const SampledDirection& microfacetNormal() const noexcept;

  //! Sample a GGX microfacet normal
  static SampledGgxNormal sample(const Float roughness,
                                 const Vector3& vin,
                                 const Vector3& normal,
                                 Sampler& sampler) noexcept;

  //! Sample the microfacet normal
  static Vector3 sampleMicrofacetNormal(const Float roughness,
                                        const Vector3& vin,
                                        Sampler& sampler) noexcept;

 private:
  template <MicrofacetGgx::GgxMethodType kMethod>
  class GgxMethod
  {
   public:
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
  };

  // Methods
  using Smith = GgxMethod<MicrofacetGgx::GgxMethodType::kSmith>;
  using VCavity = GgxMethod<MicrofacetGgx::GgxMethodType::kVCavity>;


  SampledDirection microfacet_normal_;
  Float cos_ni_,
        cos_mi_,
        cos_nm_;
};

//! \} Core

} // namespace nanairo

#include "microfacet_ggx-inl.hpp"

#endif // NANAIRO_MICROFACET_GGX_HPP
