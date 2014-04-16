/*!
  \file microfacet.hpp
  \author Sho Ikeda
  
  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_MICROFACET_HPP_
#define _NANAIRO_MICROFACET_HPP_

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

namespace nanairo {

// Forward declaration
class Sampler;
class SampledDirection;

//! Calculate microfacet reflection direction
SampledDirection getMicrofacetReflectionDirection(
    const Vector3& vin,
    const SampledDirection& microfacet_normal,
    const Float cos_theta_mi);

//! Get microfacet reflection half-vector
Vector3 getMicrofacetReflectionHalfVector(const Vector3& vin, 
                                          const Vector3& vout);

//! Get microfacet reflection inverse jacobian
Float getMicrofacetReflectionInverseJacobian(const Float cos_theta_mi);

//! Calculate microfacet refraction direction
SampledDirection getMicrofacetRefractionDirection(
    const Vector3& vin,
    const SampledDirection& microfacet_normal,
    const Float cos_theta_mi,
    const Float n,
    const Float g);

//! Get microfacet reflection half-vector
Vector3 getMicrofacetRefractionHalfVector(const Vector3& vin, 
                                          const Vector3& vout,
                                          const Float n);

//! Get microfacet reflection inverse jacobian
Float getMicrofacetRefractionInverseJacobian(const Float cos_theta_mi,
                                             const Float cos_theta_mo,
                                             const Float n);

} // namespace nanairo

#include "microfacet-inl.hpp"

#endif // _NANAIRO_MICROFACET_HPP_
