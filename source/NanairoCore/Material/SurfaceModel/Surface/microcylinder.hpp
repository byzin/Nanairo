/*!
  \file microcylinder.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MICROCYLINDER_HPP
#define NANAIRO_MICROCYLINDER_HPP

// Standard C++ library
#include <tuple>
#include <utility>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  */
class Microcylinder
{
 public:
  //! Evaluate the reflectance
  static Float evalPdf(const Vector3& vin,
                       const Vector3& vout,
                       const Vector3& normal,
                       const Float k_d,
                       const Float gamma_r,
                       const Float gamma_v) noexcept;

  //! Evaluate the reflectance
  static SampledSpectra evalReflectance(
      const Vector3& vin,
      const Vector3& vout,
      const Vector3& normal,
      const SampledSpectra& albedo,
      const Float n,
      const Float k_d,
      const Float gamma_r,
      const Float gamma_v,
      const Float rho,
      Float* pdf = nullptr) noexcept;

  //! Sample a direction
  static std::tuple<SampledDirection, SampledSpectra>
  sample(const Vector3& vin,
         const Vector3& normal,
         const SampledSpectra& albedo,
         const Float n,
         const Float k_d,
         const Float gamma_r,
         const Float gamma_v,
         const Float rho,
         Sampler& sampler) noexcept;

  //! Evaluate the azimuthal pdf
  static Float evalAzimuthalAnglePdf(const Float phi_i,
                                     const Float phi_o) noexcept;

  //! Evaluate the pdf of the surface angles
  static Float evalSurfaceAnglesPdf(const Float theta_i,
                                    const Float phi_i,
                                    const Float theta_o,
                                    const Float phi_o,
                                    const Float gamma) noexcept;

  //! Calculate the longitudinal angle pdf of the surface reflection
  static Float evalSurfaceLongitudinalAnglePdf(const Float theta_i,
                                               const Float theta_o,
                                               const Float gamma) noexcept;

  //! Evaluate the pdf of the volume angles
  static Float evalVolumeAnglesPdf(const Float theta_i,
                                   const Float phi_i,
                                   const Float theta_o,
                                   const Float phi_o,
                                   const Float k_d,
                                   const Float gamma) noexcept;

  constexpr static Float evalVolumeIsotropicLongitudinalAnglePdf(
      const Float theta_i,
      const Float theta_o) noexcept;

  //! Calculate the longitudinal angle pdf of the volumetric reflection
  static Float evalVolumeLongitudinalAnglePdf(const Float theta_i,
                                              const Float theta_o,
                                              const Float k_d,
                                              const Float gamma) noexcept;

 private:
  //! Calculate the incident and outgoing angles
  static std::tuple<Float, Float, Float, Float>
  calcAngles(const Vector3& vin, const Vector3& vout, const Vector3& normal) noexcept;

  //! Calculate the effective angle
  static Float calcEffectiveAngle(const Float theta_i, const Float theta_o) noexcept;

  //! Calculate the half-angle
  static Float calcHalfAngle(const Float theta_i, const Float theta_o) noexcept;

  //! Calculate the fresnel reflectance
  static Float calcFresnelReflectance(const Float theta_i,
                                      const Float phi_i,
                                      const Float theta_o,
                                      const Float phi_o,
                                      const Float n) noexcept;

  //! Calculate the pdf
  static Float calcPdf(const Float theta_i,
                       const Float phi_i,
                       const Float theta_o,
                       const Float phi_o,
                       const Float k_d,
                       const Float gamma_r,
                       const Float gamma_v) noexcept;

  //! Evaluate the reflectance
  static SampledSpectra calcReflectance(
      const Float theta_i,
      const Float phi_i,
      const Float theta_o,
      const Float phi_o,
      const SampledSpectra& albedo,
      const Float n,
      const Float k_d,
      const Float gamma_r,
      const Float gamma_v,
      const Float rho) noexcept;

  //! Evaluate the D function
  static Float evalD(const Float x, const Float y, const Float z) noexcept;

  //! Evaluate the gaussian function
  static Float evalGaussian(const Float mu,
                            const Float rho,
                            const Float x) noexcept;

  //! Evaluate the shadowing masking function
  static Float evalShadowingMasking(const Float phi_i,
                                    const Float phi_o,
                                    const Float rho) noexcept;

  //! Evaluate the reflectance of the surface scattering
  static Float evalSurfaceScattering(const Float theta_i,
                                     const Float phi_i,
                                     const Float theta_o,
                                     const Float phi_o,
                                     const Float gamma) noexcept;

  //! Evaluate the uni gaussian function
  static Float evalUnitGaussian(const Float mu,
                                const Float rho,
                                const Float x) noexcept;

  //! Evaluate the reflectance of the volume scattering
  static Float evalVolumeScattering(const Float theta_i,
                                    const Float phi_i,
                                    const Float theta_o,
                                    const Float phi_o,
                                    const Float n,
                                    const Float k_d,
                                    const Float gamma) noexcept;
};

/*!
  */
class SampledMicrocylinderDir
{
 public:
  //! Initialize
  SampledMicrocylinderDir(const SampledDirection& direction,
                          const Float theta_i,
                          const Float phi_i,
                          const Float theta_o,
                          const Float phi_o) noexcept;


  //! Return the direction
  const SampledDirection& direction() const noexcept;

  //! Return the phi_i
  Float phiI() const noexcept;

  //! Return the phi_o
  Float phiO() const noexcept;

  //! Sample a direction
  static SampledMicrocylinderDir sample(const Vector3& vin,
                                        const Vector3& normal,
                                        const Float k_d,
                                        const Float gamma_r,
                                        const Float gamma_v,
                                        Sampler& sampler) noexcept;

  //! Return the theta_i
  Float thetaI() const noexcept;

  //! Return the theta_o
  Float thetaO() const noexcept;

 private:
  //! Sample a direction angles
  static std::tuple<Float, Float, Float> sampleAngles(
      const Float theta_i,
      const Float phi_i,
      const Float k_d,
      const Float gamma_r,
      const Float gamma_v,
      Sampler& sampler) noexcept;

  //! Sample a azimuthal angle
  static std::tuple<Float, Float> sampleAzimuthalAngle(
      const Float phi_i,
      Sampler& sampler) noexcept;

  //! Sample a direction angles
  static std::tuple<Float, Float, Float> sampleSurfaceAngles(
      const Float theta_i,
      const Float phi_i,
      const Float gamma,
      Sampler& sampler) noexcept;

  //! Sample a longitudinal angle
  static std::tuple<Float, Float> sampleSurfaceLongitudinalAngle(
      const Float theta_i,
      const Float gamma,
      Sampler& sampler) noexcept;

  //! Sample a direction angles
  static std::tuple<Float, Float, Float> sampleVolumeAngles(
      const Float theta_i,
      const Float phi_i,
      const Float k_d,
      const Float gamma,
      Sampler& sampler) noexcept;

  //! Sample a longitudinal angle
  static std::tuple<Float, Float> sampleVolumeIsotropicLongitudinalAngle(
      const Float theta_i,
      Sampler& sampler) noexcept;

  //! Sample a longitudinal angle
  static std::tuple<Float, Float> sampleVolumeLongitudinalAngle(
      const Float theta_i,
      const Float k_d,
      const Float gamma,
      Sampler& sampler) noexcept;


  SampledDirection direction_;
  Float theta_i_;
  Float phi_i_;
  Float theta_o_;
  Float phi_o_;
};

//! \} Core

} // namespace nanairo

#include "microcylinder-inl.hpp"

#endif // NANAIRO_MICROCYLINDER_HPP
