/*!
  \file microcylinder-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MICROCYLINDER_INL_HPP
#define NANAIRO_MICROCYLINDER_INL_HPP

#include "microcylinder.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/const_math.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "fresnel.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  */
inline
Float Microcylinder::evalPdf(const Vector3& vin,
                             const Vector3& vout,
                             const Vector3& normal,
                             const Float k_d,
                             const Float gamma_r,
                             const Float gamma_v) noexcept
{
  const auto angles = calcAngles(vin, vout, normal);
  const Float theta_i = std::get<0>(angles);
  const Float phi_i = std::get<1>(angles);
  const Float theta_o = std::get<2>(angles);
  const Float phi_o = std::get<3>(angles);
  return calcPdf(theta_i, phi_i, theta_o, phi_o, k_d, gamma_r, gamma_v);
}

/*!
  */
inline
SampledSpectra Microcylinder::evalReflectance(
    const Vector3& vin,
    const Vector3& vout,
    const Vector3& normal,
    const SampledSpectra& albedo,
    const Float n,
    const Float k_d,
    const Float gamma_r,
    const Float gamma_v,
    const Float rho,
    Float* pdf) noexcept
{
  const auto angles = calcAngles(vin, vout, normal);
  const Float theta_i = std::get<0>(angles);
  const Float phi_i = std::get<1>(angles);
  const Float theta_o = std::get<2>(angles);
  const Float phi_o = std::get<3>(angles);

  // Calculate the direction pdf
  if (pdf != nullptr)
    *pdf = calcPdf(theta_i, phi_i, theta_o, phi_o, k_d, gamma_r, gamma_v);

  // Calculate the reflectance
  const auto f = calcReflectance(theta_i, phi_i, theta_o, phi_o, albedo,
                                 n, k_d, gamma_r, gamma_v, rho);
  return f;
}

/*!
  */
inline
std::tuple<SampledDirection, SampledSpectra>
Microcylinder::sample(const Vector3& vin,
                      const Vector3& normal,
                      const SampledSpectra& albedo,
                      const Float n,
                      const Float k_d,
                      const Float gamma_r,
                      const Float gamma_v,
                      const Float rho,
                      Sampler& sampler) noexcept
{
  // Sample a reflection direction
  const auto sample_result = SampledMicrocylinderDir::sample(vin, normal, k_d,
                                                             gamma_r, gamma_v,
                                                             sampler);
  const auto& sampled_vout = sample_result.direction();
  const Float theta_i = sample_result.thetaI();
  const Float phi_i = sample_result.phiI();
  const Float theta_o = sample_result.thetaO();
  const Float phi_o = sample_result.phiO();
  // Calculate the reflectance
  const auto reflectance = calcReflectance(theta_i, phi_i, theta_o, phi_o, albedo,
                                           n, k_d, gamma_r, gamma_v, rho);
  const Float cos_n = zisc::dot(normal, sampled_vout.direction());
  ZISC_ASSERT(zisc::isInBounds(cos_n, 0.0, 1.0),
              "The dot(n, o) is out of range [0, 1].");
  const auto weight = reflectance * (cos_n * sampled_vout.inversePdf());
  return std::make_tuple(sampled_vout, weight);
}


/*!
  */
inline
Float Microcylinder::calcEffectiveAngle(const Float theta_i,
                                        const Float theta_o) noexcept
{
  return 0.5 * (theta_i - theta_o);
}

/*!
  */
inline
Float Microcylinder::calcHalfAngle(const Float theta_i,
                                   const Float theta_o) noexcept
{
  return 0.5 * (theta_i + theta_o);
}

/*!
  */
inline
Float Microcylinder::evalAzimuthalAnglePdf(const Float /* phi_i */,
                                           const Float phi_o) noexcept
{
  return 0.5 * zisc::cos(phi_o);
}

/*!
  */
inline
Float Microcylinder::evalSurfaceAnglesPdf(const Float theta_i,
                                          const Float phi_i,
                                          const Float theta_o,
                                          const Float phi_o,
                                          const Float gamma) noexcept
{
  const Float theta_pdf = evalSurfaceLongitudinalAnglePdf(theta_i, theta_o, gamma);
  const Float phi_pdf = evalAzimuthalAnglePdf(phi_i, phi_o);
  return theta_pdf * phi_pdf;
}

/*!
  */
inline
Float Microcylinder::evalSurfaceLongitudinalAnglePdf(const Float theta_i,
                                                     const Float theta_o,
                                                     const Float gamma) noexcept
{
  constexpr Float half_pi = 0.5 * zisc::kPi<Float>;
  const Float a = zisc::atan((half_pi - theta_i) / gamma);
  const Float b = zisc::atan((-half_pi - theta_i) / gamma);
  const Float pdf =
      gamma / ((zisc::power<2>(theta_o - theta_i) + zisc::power<2>(gamma)) *
               zisc::cos(theta_o) * (a - b));
  return pdf;
}

/*!
  */
inline
Float Microcylinder::evalVolumeAnglesPdf(const Float theta_i,
                                         const Float phi_i,
                                         const Float theta_o,
                                         const Float phi_o,
                                         const Float k_d,
                                         const Float gamma) noexcept
{
  const Float theta_pdf = evalVolumeLongitudinalAnglePdf(theta_i, theta_o,
                                                         k_d, gamma);
  const Float phi_pdf = evalAzimuthalAnglePdf(phi_i, phi_o);
  return theta_pdf * phi_pdf;
}

/*!
  */
inline
constexpr Float Microcylinder::evalVolumeIsotropicLongitudinalAnglePdf(
    const Float /* theta_i */,
    const Float /* theta_o */) noexcept
{
  return 0.5;
}

/*!
  */
inline
Float Microcylinder::evalVolumeLongitudinalAnglePdf(const Float theta_i,
                                                    const Float theta_o,
                                                    const Float k_d,
                                                    const Float gamma) noexcept
{
  const Float iso_pdf = evalVolumeIsotropicLongitudinalAnglePdf(theta_i, theta_o);
  const Float aniso_pdf = evalSurfaceLongitudinalAnglePdf(theta_i, theta_o, gamma);
  return k_d * iso_pdf + (1.0 - k_d) * aniso_pdf;
}

/*!
  */
inline
std::tuple<Float, Float, Float, Float>
Microcylinder::calcAngles(const Vector3& vin,
                          const Vector3& vout,
                          const Vector3& normal) noexcept
{
  using zisc::kPi;

  // Change of basis of the incident vector
  const auto transformation = Transformation::makeChangeOfBasisToLocal(normal);
  // Incident vector
  const auto incident_vector = transformation * -vin;
  ZISC_ASSERT(isUnitVector(incident_vector), "Incident vector isn't unit vector.");
  const Float theta_i = zisc::asin(incident_vector[0]);
  const Float phi_i = zisc::atan(incident_vector[1] / incident_vector[2]);
  ZISC_ASSERT(zisc::isInClosedBounds(theta_i, -0.5*kPi<Float>, 0.5*kPi<Float>),
              "The theta_i is out of range [-pi/2, pi/2].");
  ZISC_ASSERT(zisc::isInClosedBounds(phi_i, -0.5*kPi<Float>, 0.5*kPi<Float>),
              "The phi_i is out of range [-pi/2, pi/2].");
  // Outgoing vector
  const auto outgoing_vector = transformation * vout;
  ZISC_ASSERT(isUnitVector(outgoing_vector), "Outgoing vector isn't unit vector.");
  const Float theta_o = zisc::asin(outgoing_vector[0]);
  const Float phi_o = zisc::atan(outgoing_vector[1] / outgoing_vector[2]);
  ZISC_ASSERT(zisc::isInClosedBounds(theta_o, -0.5*kPi<Float>, 0.5*kPi<Float>),
              "The theta_o is out of range [-pi/2, pi/2].");
  ZISC_ASSERT(zisc::isInClosedBounds(phi_o, -0.5*kPi<Float>, 0.5*kPi<Float>),
              "The phi_o is out of range [-pi/2, pi/2].");
  return std::make_tuple(theta_i, phi_i, theta_o, phi_o);
}

/*!
  */
inline
Float Microcylinder::calcFresnelReflectance(const Float theta_i,
                                            const Float phi_i,
                                            const Float theta_o,
                                            const Float phi_o,
                                            const Float n) noexcept
{
  const Float theta_d = calcEffectiveAngle(theta_i, theta_o);
  const Float phi_d = phi_i - phi_o;
  const Float cos_n = zisc::cos(theta_d) * zisc::cos(0.5 * phi_d);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_n, 0.0, 1.0),
              "The cos(n) isn't [0, 1].");
  const Float fresnel = Fresnel::evalFresnel(n, cos_n);
  return fresnel;
}

/*!
  */
inline
Float Microcylinder::calcPdf(const Float theta_i,
                             const Float phi_i,
                             const Float theta_o,
                             const Float phi_o,
                             const Float k_d,
                             const Float gamma_r,
                             const Float gamma_v) noexcept
{
  constexpr Float p = 0.5;
  const Float surface_pdf = evalSurfaceAnglesPdf(theta_i, phi_i, theta_o, phi_o,
                                                 gamma_r);
  const Float volume_pdf = evalVolumeAnglesPdf(theta_i, phi_i, theta_o, phi_o,
                                               k_d, gamma_v);
  return p * surface_pdf + (1.0 - p) * volume_pdf;
}

/*!
  */
inline
SampledSpectra Microcylinder::calcReflectance(
    const Float theta_i,
    const Float phi_i,
    const Float theta_o,
    const Float phi_o,
    const SampledSpectra& albedo,
    const Float n,
    const Float k_d,
    const Float gamma_r,
    const Float gamma_v,
    const Float rho) noexcept
{
  const Float theta_d = calcEffectiveAngle(theta_i, theta_o);
  const Float k = evalShadowingMasking(phi_i, phi_o, rho) /
                  zisc::power<2>(zisc::cos(theta_d));
  // Calculate the reflectance
  const Float fresnel = calcFresnelReflectance(theta_i, phi_i, theta_o, phi_o, n);
  const Float f_r = evalSurfaceScattering(theta_i, phi_i, theta_o, phi_o, gamma_r);
  SampledSpectra reflectance{albedo.wavelengths(), fresnel * f_r * k};
  if (fresnel != 1.0) {
    const Float f_v = evalVolumeScattering(theta_i, phi_i, theta_o, phi_o,
                                           n, k_d, gamma_v);
    reflectance = reflectance + ((1.0 - fresnel) * f_v * k) * albedo;
  }
  return reflectance;
}

/*!
  */
inline
Float Microcylinder::evalD(const Float x, const Float y, const Float z) noexcept
{
  return (1.0 - x) * y * z + x * zisc::min(y, z);
}

/*!
  */
inline
Float Microcylinder::evalGaussian(const Float mu,
                                  const Float rho,
                                  const Float x) noexcept
{
  const Float t = -zisc::power<2>(x - mu) / (2.0 * zisc::power<2>(rho));
  return zisc::exp(t);
}

/*!
  */
inline
Float Microcylinder::evalShadowingMasking(const Float phi_i,
                                          const Float phi_o,
                                          const Float rho) noexcept
{
  const Float cos_phi_i = zisc::cos(phi_i);
  const Float cos_phi_o = zisc::cos(phi_o);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_phi_i, 0.0, 1.0),
              "The phi_i is out of range [-pi/2, pi/2].");
  ZISC_ASSERT(zisc::isInClosedBounds(cos_phi_o, 0.0, 1.0),
              "The phi_i is out of range [-pi/2, pi/2].");
  const Float phi_d = phi_i - phi_o;
  const Float g = evalGaussian(0.0, rho, phi_d);
  const Float d = evalD(g, cos_phi_i, cos_phi_o);
  return d;
}

/*!
  */
inline
Float Microcylinder::evalSurfaceScattering(const Float theta_i,
                                           const Float phi_i,
                                           const Float theta_o,
                                           const Float phi_o,
                                           const Float gamma) noexcept
{
  const Float phi_d = phi_i - phi_o;
  const Float theta_h = calcHalfAngle(theta_i, theta_o);
  return zisc::cos(0.5 * phi_d) * evalUnitGaussian(0.0, gamma, theta_h);
}

/*!
  */
inline
Float Microcylinder::evalUnitGaussian(const Float mu,
                                      const Float rho,
                                      const Float x) noexcept
{
  constexpr Float root_pi = zisc::constant::sqrt(2.0 * zisc::kPi<Float>);
  return evalGaussian(mu, rho, x) / (root_pi * rho);
}

/*!
  */
inline
Float Microcylinder::evalVolumeScattering(const Float theta_i,
                                          const Float phi_i,
                                          const Float theta_o,
                                          const Float phi_o,
                                          const Float n,
                                          const Float k_d,
                                          const Float gamma) noexcept
{
  // Fresnel
  Float fresnel;
  {
    const Float theta_d = calcEffectiveAngle(theta_i, theta_o);
    const Float phi_d = phi_i - phi_o;
    const Float cos_n = zisc::cos(theta_d) * zisc::cos(0.5 * phi_d);
    const Float inv_n = zisc::invert(n);
    const Float sin_m = inv_n * zisc::sqrt(1.0 - zisc::power<2>(cos_n));
    const Float cos_m = zisc::sqrt(1.0 - zisc::power<2>(sin_m));
    ZISC_ASSERT(zisc::isInClosedBounds(cos_m, 0.0, 1.0),
                "The cos_m is out of range [0, 1].");
    fresnel = Fresnel::evalFresnel(n, cos_m);
  }
  const Float theta_h = calcHalfAngle(theta_i, theta_o);
  return (fresnel != 1.0)
      ? (1.0 - fresnel) *
        (k_d + (1.0 - k_d) * evalUnitGaussian(0.0, gamma, theta_h)) /
        (zisc::cos(theta_i) + zisc::cos(theta_o))
      : 0.0;
}

/*!
  */
inline
SampledMicrocylinderDir::SampledMicrocylinderDir(const SampledDirection& direction,
                                                 const Float theta_i,
                                                 const Float phi_i,
                                                 const Float theta_o,
                                                 const Float phi_o) noexcept :
    direction_{direction},
    theta_i_{theta_i},
    phi_i_{phi_i},
    theta_o_{theta_o},
    phi_o_{phi_o}
{
}

/*!
  */
inline
const SampledDirection& SampledMicrocylinderDir::direction() const noexcept
{
  return direction_;
}

/*!
  */
inline
Float SampledMicrocylinderDir::phiI() const noexcept
{
  return phi_i_;
}

/*!
  */
inline
Float SampledMicrocylinderDir::phiO() const noexcept
{
  return phi_o_;
}

/*!
  */
inline
Float SampledMicrocylinderDir::thetaI() const noexcept
{
  return theta_i_;
}

/*!
  */
inline
Float SampledMicrocylinderDir::thetaO() const noexcept
{
  return theta_o_;
}

} // namespace nanairo

#endif // NANAIRO_MICROCYLINDER_INL_HPP
