/*!
  \file microfacet.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MICROFACET_HPP
#define NANAIRO_MICROFACET_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

// Forward declaration
class Sampler;
class SampledDirection;

//! \addtogroup Core
//! \{

/*!
  */
class Microfacet
{
 public:
  //! The microsurface profile
  enum class MicrosurfaceProfile : int
  {
    kSmith,
    kVCavity
  };


  template <MicrosurfaceProfile kProfile>
  class Microsurface
  {
   public:
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
  };


  using SmithMicrosurface = Microsurface<MicrosurfaceProfile::kSmith>;
  using VCavityMicrosurface = Microsurface<MicrosurfaceProfile::kVCavity>;


  //! Calculate microfacet reflection direction
  static SampledDirection calcReflectionDirection(
      const Vector3& vin,
      const SampledDirection& sampled_m_normal) noexcept;

  //! Calculate microfacet reflection half-vector
  static Vector3 calcReflectionHalfVector(const Vector3& vin,
                                          const Vector3& vout) noexcept;

  //! Calculate microfacet reflection inverse jacobian
  static Float calcReflectionInverseJacobian(const Float cos_mi) noexcept;

  //! Calculate microfacet refraction direction
  static SampledDirection calcRefractionDirection(
      const Vector3& vin,
      const SampledDirection& sampled_m_normal,
      const Float n,
      const Float g) noexcept;

  //! Calculate microfacet reflection half-vector
  static Vector3 calcRefractionHalfVector(const Vector3& vin,
                                          const Vector3& vout,
                                          const Float n) noexcept;

  //! Calculate microfacet reflection inverse jacobian
  static Float calcRefractionInverseJacobian(const Float cos_mi,
                                             const Float cos_mo,
                                             const Float n) noexcept;

 protected:
  //! Calculate the (projected roughness)^2 * (tangent theta)^2
  static Float calcRoughness2Tan2(const Float roughness_x,
                                  const Float roughness_y,
                                  const Vector3& v) noexcept;
};

//! \} Core

} // namespace nanairo

#include "microfacet-inl.hpp"

#endif // NANAIRO_MICROFACET_HPP
