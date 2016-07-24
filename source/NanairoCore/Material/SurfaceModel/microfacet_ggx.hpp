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
#include <utility>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class SampledDirection;
template <uint> class SampledSpectra;
class Sampler;

//! Evaluate GGX D
Float evaluateGgxD(const Float roughness, const Float cos_theta_nm) noexcept;

//! Evaluate GGX G1 term
Float evaluateGgxG1(const Float roughness, 
                    const Float cos_theta_n, 
                    const Float cos_theta_m,
                    const Float cos_theta_nm) noexcept;

//! Evaluate GGX G2 term
Float evaluateGgxG2(const Float roughness,
                    const Float cos_theta_ni,
                    const Float cos_theta_no,
                    const Float cos_theta_mi,
                    const Float cos_theta_mo,
                    const Float cos_theta_nm) noexcept;

//! Evaluate the GGX reflectance
template <uint kSampleSize>
SampledSpectra<kSampleSize> evaluateGgxReflectance(
    const Float roughness,
    const Vector3& vin,
    const Vector3& vout,
    const Vector3& normal,
    const SampledSpectra<kSampleSize>& r0,
    Float* pdf = nullptr) noexcept;

//! Evaluate the GGX reflection pdf
Float evaluateGgxReflectionPdf(const Float roughness,
                               const Vector3& vin,
                               const Vector3& vout,
                               const Vector3& normal) noexcept;

//! Evaluate the GGX reflectance of a dielectric material
Float evaluateGgxReflectance(const Float roughness,
                             const Vector3& vin,
                             const Vector3& vout,
                             const Vector3& normal,
                             const Float n,
                             const Float cos_theta_no,
                             Float* pdf = nullptr) noexcept;

//! Evaluate the GGX transmittance of a dielectric material
Float evaluateGgxTransmittance(const Float roughness,
                               const Vector3& vin,
                               const Vector3& vout,
                               const Vector3& normal,
                               const Float n,
                               const Float cos_theta_no,
                               Float* pdf = nullptr) noexcept;

//! Evaluate the GGX reflection pdf of a dielectric material
Float evaluateGgxDielectricReflectionPdf(const Float roughness,
                                         const Vector3& vin,
                                         const Vector3& vout,
                                         const Vector3& normal,
                                         const Float n) noexcept;

//! Evaluate the GGX reflection pdf of a dielectric material
Float evaluateGgxDielectricRefractionPdf(const Float roughness,
                                         const Vector3& vin,
                                         const Vector3& vout,
                                         const Vector3& normal,
                                         const Float n) noexcept;

//! Evaluate the GGX weight
Float evaluateGgxWeight(const Float roughness,
                        const Float cos_theta_ni,
                        const Float cos_theta_no,
                        const Float cos_theta_mi,
                        const Float cos_theta_mo,
                        const Float cos_theta_nm) noexcept;

//! Sample GGX microfacet normal
SampledDirection sampleGgxMicrofacetNormal(const Float roughness,
                                           const Vector3& vin,
                                           const Vector3& normal,
                                           Sampler& sampler,
                                           Float* cos_ni,
                                           Float* cos_mi,
                                           Float* cos_nm) noexcept;

// #define NANAIRO_GGX_V_CAVITY
#define NANAIRO_GGX_SMITH

} // namespace nanairo

#include "microfacet_ggx-inl.hpp"

#endif // NANAIRO_MICROFACET_GGX_HPP
