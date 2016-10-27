/*!
  \file lambert_brdf-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LAMBERT_BRDF_INL_HPP
#define NANAIRO_LAMBERT_BRDF_INL_HPP

#include "lambert_brdf.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"


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
LambertBrdf<kSampleSize>::LambertBrdf(const Spectra& reflectance) noexcept :
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
    const Wavelengths& /* wavelemgths */) const noexcept
{
  constexpr Float k = 1.0 / zisc::kPi<Float>;
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
    const Wavelengths& /* wavelemgths */) const noexcept -> Spectra
{
  constexpr Float k = 1.0 / zisc::kPi<Float>;
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
    const Wavelengths& /* wavelemgths */) const noexcept -> std::tuple<Spectra, Float>
{
  constexpr Float k = 1.0 / zisc::kPi<Float>;
  const Float cos_theta_no = zisc::dot(normal, *vout);
  ZISC_ASSERT(zisc::isInBounds(cos_theta_no, 0.0, 1.0),
              "The cos theta_{n, o} isn't [0, 1].");
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
    Sampler& sampler) const noexcept -> std::tuple<SampledDirection, Spectra>
{
  const auto vout = sampleDirectionOnHemisphere<1>(normal, sampler);
  return std::make_tuple(std::move(vout), reflectance_);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
bool LambertBrdf<kSampleSize>::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo

#endif // NANAIRO_LAMBERT_BRDF_INL_HPP
