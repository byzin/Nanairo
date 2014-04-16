/*!
  \file microfacet_ggx.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_MICROFACET_GGX_HPP_
#define _NANAIRO_MICROFACET_GGX_HPP_

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
Float evaluateGgxD(const Float roughness, const Float cos_theta_nm);

//! Evaluate GGX G1 term
Float evaluateGgxG1(const Float roughness, 
                    const Float cos_theta_n, 
                    const Float cos_theta_m,
                    const Float cos_theta_nm);

//! Evaluate GGX G2 term
Float evaluateGgxG2(const Float roughness,
                    const Float cos_theta_ni,
                    const Float cos_theta_no,
                    const Float cos_theta_mi,
                    const Float cos_theta_mo,
                    const Float cos_theta_nm);

//! Evaluate the GGX reflectance
template <uint kSampleSize>
SampledSpectra<kSampleSize> evaluateGgxReflectance(
    const Float roughness,
    const Vector3& vin,
    const Vector3& vout,
    const Vector3& normal,
    const SampledSpectra<kSampleSize>& r0,
    Float* pdf = nullptr);

//! Evaluate the GGX reflection pdf
Float evaluateGgxReflectionPdf(const Float roughness,
                               const Vector3& vin,
                               const Vector3& vout,
                               const Vector3& normal);

//! Evaluate the GGX reflectance of a dielectric material
Float evaluateGgxReflectance(const Float roughness,
                             const Vector3& vin,
                             const Vector3& vout,
                             const Vector3& normal,
                             const Float n,
                             const Float cos_theta_no,
                             Float* pdf = nullptr);

//! Evaluate the GGX transmittance of a dielectric material
Float evaluateGgxTransmittance(const Float roughness,
                               const Vector3& vin,
                               const Vector3& vout,
                               const Vector3& normal,
                               const Float n,
                               const Float cos_theta_no,
                               Float* pdf = nullptr);

//! Evaluate the GGX reflection pdf of a dielectric material
Float evaluateGgxDielectricReflectionPdf(const Float roughness,
                                         const Vector3& vin,
                                         const Vector3& vout,
                                         const Vector3& normal,
                                         const Float n);

//! Evaluate the GGX reflection pdf of a dielectric material
Float evaluateGgxDielectricRefractionPdf(const Float roughness,
                                         const Vector3& vin,
                                         const Vector3& vout,
                                         const Vector3& normal,
                                         const Float n);

//! Evaluate the GGX weight
Float evaluateGgxWeight(const Float roughness,
                        const Float cos_theta_ni,
                        const Float cos_theta_no,
                        const Float cos_theta_mi,
                        const Float cos_theta_mo,
                        const Float cos_theta_nm);

//! Sample GGX microfacet normal
SampledDirection sampleGgxMicrofacetNormal(const Float roughness,
                                           const Vector3& vin,
                                           const Vector3& normal,
                                           Sampler& sampler,
                                           Float* cos_ni,
                                           Float* cos_mi,
                                           Float* cos_nm);

// #define _NANAIRO_GGX_V_CAVITY_
#define _NANAIRO_GGX_SMITH_

} // namespace nanairo

#include "microfacet_ggx-inl.hpp"

#endif // _NANAIRO_MICROFACET_GGX_HPP_
