/*!
  \file lambert_brdf-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_LAMBERT_BRDF_INL_HPP_
#define _NANAIRO_LAMBERT_BRDF_INL_HPP_

#include "lambert_brdf.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/floating_point.hpp"


namespace nanairo {

// Forward declaration
class SampledDirection;
class Sampler;
template <uint> class WavelengthSamples;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
LambertBrdf<kSampleSize>::LambertBrdf(const Spectra& reflectance) :
    reflectance_{reflectance}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
Float LambertBrdf<kSampleSize>::evaluatePdf(
    const Vector3* /* vin */,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelemgths */) const
{
  constexpr Float k = 1.0 / zisc::kPi;
  const Float cos_theta = zisc::dot(normal, *vout);
  return k * cos_theta;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto LambertBrdf<kSampleSize>::evaluateRadiance(
    const Vector3* /* vin */,
    const Vector3* /* vout */,
    const Vector3& /* normal */,
    const Wavelengths& /* wavelemgths */) const -> Spectra
{
  constexpr Float k = 1.0 / zisc::kPi;
  return k * reflectance_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto LambertBrdf<kSampleSize>::evaluateRadianceAndPdf(
    const Vector3* /* vin */,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelemgths */) const -> std::tuple<Spectra, Float>
{
  constexpr Float k = 1.0 / zisc::kPi;
  const Float cos_theta_no = zisc::dot(normal, *vout);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_no),
              "The cos theta_{n, o} must be [0, 1].");
  return std::make_tuple(k * reflectance_, k * cos_theta_no);;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto LambertBrdf<kSampleSize>::sample(
    const Vector3* /* vin */,
    const Vector3& normal,
    const Wavelengths& /* wavelemgths */,
    Sampler& sampler) const -> std::tuple<SampledDirection, Spectra>
{
  const auto vout = sampleDirectionOnHemisphere<1>(normal, sampler);
  return std::make_tuple(std::move(vout), reflectance_);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
bool LambertBrdf<kSampleSize>::wavelengthIsSelected() const
{
  return false;
}

} // namespace nanairo

#endif // _NANAIRO_LAMBERT_BRDF_INL_HPP_
