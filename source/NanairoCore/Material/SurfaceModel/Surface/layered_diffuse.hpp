/*!
  \file layered_diffuse.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LAYERED_DIFFUSE_HPP
#define NANAIRO_LAYERED_DIFFUSE_HPP

// Standard C++ library
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
class LayeredDiffuse
{
 public:
  //! Evaluate the pdf
  static Float evalPdf(const Float roughness_x,
                       const Float roughness_y,
                       const Vector3& vin,
                       const Vector3& vout,
                       const Float n,
                       const Float k_d,
                       const Float re) noexcept;

  //! Evaluate the reflectance
  static Float evalReflectance(const Float roughness_x,
                               const Float roughness_y,
                               const Vector3& vin,
                               const Vector3& vout,
                               const Float n,
                               const Float k_d,
                               const Float re,
                               Sampler& sampler,
                               Float* pdf = nullptr) noexcept;

  //! Sample a reflection direction and evaluate the direction weight
  static std::tuple<SampledDirection, Float> sample(
      const Float roughness_x,
      const Float roughness_y,
      const Vector3& vin,
      const Float n,
      const Float k_d,
      const Float re,
      Sampler& sampler) noexcept;

  //! Calculate the internal reflectance
  static Float calcRi(const Float n) noexcept;

  //! Calculate the internal reflectance
  static Float calcRe(const Float n) noexcept;

 private:
  //! Evaluate the pdf ot body direction
  static Float evalBodyPdf(const Float cos_no) noexcept;

  //! Evaluate body reflectance
  static Float evalBodyReflectance(const Float roughness_x,
                                   const Float roughness_y,
                                   const Vector3& vin,
                                   const Vector3& vout,
                                   const Float n,
                                   const Float k_d,
                                   const Float ri,
                                   Sampler& sampler,
                                   Float* pdf = nullptr) noexcept;

  //! Calculate pure body reflectance
  static Float calcPureBodyReflectance(const Float cos_mi,
                                       const Float cos_mo,
                                       const Float n,
                                       const Float k_d,
                                       const Float ri) noexcept;

  //! Calculate the total body reflectance
  static Float calcTotalBodyReflectance(const Float n,
                                        const Float k_d,
                                        const Float ri) noexcept;

  //! Calculate the internal reflectance
  static Float toRi(const Float n, const Float re) noexcept;

  //! Calculate the internal reflectance
  static Float toRe(const Float n, const Float ri) noexcept;
};

//! \} Core

} // namespace nanairo

#include "layered_diffuse-inl.hpp"

#endif // NANAIRO_LAYERED_DIFFUSE_HPP
