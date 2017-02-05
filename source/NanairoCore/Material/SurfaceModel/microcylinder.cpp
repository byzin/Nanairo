/*!
  \file microcylinder.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "microcylinder.hpp"
// Standard C++ library
#include <tuple>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo {

/*!
  */
SampledMicrocylinderDir SampledMicrocylinderDir::sample(const Vector3& vin,
                                                        const Vector3& normal,
                                                        const Float k_d,
                                                        const Float gamma_r,
                                                        const Float gamma_v,
                                                        Sampler& sampler) noexcept
{
  using zisc::kPi;

  // Change of basis of the incident vector
  const auto transformation = Transformation::makeChangeOfBasisToLocal(normal);
  // Incident vector
  const auto incident_vector = transformation * -vin;
  ZISC_ASSERT(isUnitVector(incident_vector), "Incident vector isn't unit vector.");
  const Float theta_i = zisc::asin(incident_vector[0]);
  const Float phi_i = zisc::atan(incident_vector[1] / incident_vector[2]);
  ZISC_ASSERT(zisc::isInBounds(phi_i, -0.5*kPi<Float>, 0.5*kPi<Float>),
              "The phi_i is out of range [-pi/2, pi/2).");
  ZISC_ASSERT(zisc::isInBounds(theta_i, -0.5*kPi<Float>, 0.5*kPi<Float>),
              "The theta_i is out of range [-pi/2, pi/2).");
  // Sample a direction
  const auto result = sampleAngles(theta_i, phi_i, k_d, gamma_r, gamma_v, sampler);
  // Ohtgoing vector
  const Float theta_o = std::get<0>(result);
  const Float phi_o = std::get<1>(result);
  const Float cos_o = zisc::cos(theta_o);
  Vector3 vout{zisc::sin(theta_o),
               cos_o * zisc::sin(phi_o),
               cos_o * zisc::cos(phi_o)};
  vout = transformation.transposedMatrix() * vout;
  const Float pdf = std::get<2>(result);
  ZISC_ASSERT(isUnitVector(vout), "The sampled vout isn't unit vector.");
  // Calculate the direction pdf
  return SampledMicrocylinderDir{SampledDirection{vout, zisc::invert(pdf)},
                                 theta_i,
                                 phi_i,
                                 theta_o,
                                 phi_o};
}

/*!
  */
inline
std::tuple<Float, Float, Float> SampledMicrocylinderDir::sampleAngles(
    const Float theta_i,
    const Float phi_i,
    const Float k_d,
    const Float gamma_r,
    const Float gamma_v,
    Sampler& sampler) noexcept
{
  constexpr Float p = 0.5;
  const bool is_surface_reflection = sampler.sample() < p;
  auto result = (is_surface_reflection)
      ? sampleSurfaceAngles(theta_i, phi_i, gamma_r, sampler)
      : sampleVolumeAngles(theta_i, phi_i, k_d, gamma_v, sampler);
  const Float theta_o = std::get<0>(result),
              phi_o = std::get<1>(result);
  const Float surface_pdf = (is_surface_reflection)
      ? std::get<2>(result)
      : Microcylinder::evalSurfaceAnglesPdf(theta_i, phi_i, theta_o, phi_o,
                                            gamma_r);
  const Float volume_pdf = (!is_surface_reflection)
      ? std::get<2>(result)
      : Microcylinder::evalVolumeAnglesPdf(theta_i, phi_i, theta_o, phi_o,
                                           k_d, gamma_v);
  std::get<2>(result) = p * surface_pdf + (1.0 - p) * volume_pdf;
  return result;
}

/*!
  */
inline
std::tuple<Float, Float> SampledMicrocylinderDir::sampleAzimuthalAngle(
    const Float phi_i,
    Sampler& sampler) noexcept
{
  const Float u = sampler.sample();
  const Float phi_o = zisc::asin(2.0 * u - 1.0);
  const Float pdf = Microcylinder::evalAzimuthalAnglePdf(phi_i, phi_o);
  return std::make_tuple(phi_o, pdf);
}

/*!
  */
inline
std::tuple<Float, Float, Float> SampledMicrocylinderDir::sampleSurfaceAngles(
    const Float theta_i,
    const Float phi_i,
    const Float gamma,
    Sampler& sampler) noexcept
{
  const auto result1 = sampleSurfaceLongitudinalAngle(theta_i, gamma, sampler);
  const auto result2 = sampleAzimuthalAngle(phi_i, sampler);
  const Float theta_o = std::get<0>(result1);
  const Float phi_o = std::get<0>(result2);
  const Float pdf = std::get<1>(result1) * std::get<1>(result2);
  return std::make_tuple(theta_o, phi_o, pdf);
}

/*!
  */
inline
std::tuple<Float, Float> SampledMicrocylinderDir::sampleSurfaceLongitudinalAngle(
    const Float theta_i,
    const Float gamma,
    Sampler& sampler) noexcept
{
  constexpr Float half_pi = 0.5 * zisc::kPi<Float>;
  const Float a = zisc::atan((half_pi - theta_i) / gamma);
  const Float b = zisc::atan((-half_pi - theta_i) / gamma);
  const Float u = sampler.sample();
  const Float theta_o = gamma * zisc::tan(u * (a - b) + b) + theta_i;
  const Float pdf =
      gamma / ((zisc::power<2>(theta_i - theta_o) + zisc::power<2>(gamma)) *
               (zisc::cos(theta_o) * (a - b)));
  return std::make_tuple(theta_o, pdf);
}

/*!
  */
inline
std::tuple<Float, Float, Float> SampledMicrocylinderDir::sampleVolumeAngles(
    const Float theta_i,
    const Float phi_i,
    const Float k_d,
    const Float gamma,
    Sampler& sampler) noexcept
{
  const auto result1 = sampleVolumeLongitudinalAngle(theta_i, k_d, gamma, sampler);
  const auto result2 = sampleAzimuthalAngle(phi_i, sampler);
  const Float theta_o = std::get<0>(result1);
  const Float phi_o = std::get<0>(result2);
  const Float pdf = std::get<1>(result1) * std::get<1>(result2);
  return std::make_tuple(theta_o, phi_o, pdf);
}

/*!
  */
inline
std::tuple<Float, Float> SampledMicrocylinderDir::
sampleVolumeIsotropicLongitudinalAngle(
    const Float theta_i,
    Sampler& sampler) noexcept
{
  const Float u = sampler.sample();
  const Float theta_o = zisc::asin(2.0 * u - 1.0);
  const Float pdf = Microcylinder::evalVolumeIsotropicLongitudinalAnglePdf(theta_i,
                                                                           theta_o);
  return std::make_tuple(theta_o, pdf);
}

/*!
  */
inline
std::tuple<Float, Float> SampledMicrocylinderDir::sampleVolumeLongitudinalAngle(
    const Float theta_i,
    const Float k_d,
    const Float gamma,
    Sampler& sampler) noexcept
{
  ZISC_ASSERT(zisc::isInClosedBounds(k_d, 0.0, 1.0), "The k_d isn't [0, 1].");
  const bool is_isotropic_reflection = sampler.sample() < k_d;
  auto result = (is_isotropic_reflection)
      ? sampleVolumeIsotropicLongitudinalAngle(theta_i, sampler)
      : sampleSurfaceLongitudinalAngle(theta_i, gamma, sampler);
  const Float theta_o = std::get<0>(result);
  const Float iso_pdf = (is_isotropic_reflection)
      ? std::get<1>(result)
      : Microcylinder::evalVolumeIsotropicLongitudinalAnglePdf(theta_i, theta_o);
  const Float aniso_pdf = (!is_isotropic_reflection)
      ? std::get<1>(result)
      : Microcylinder::evalSurfaceLongitudinalAnglePdf(theta_i, theta_o, gamma);
  std::get<1>(result) = k_d * iso_pdf + (1.0 - k_d) * aniso_pdf;
  return result;
}

} // namespace nanairo
