/*!
  \file lambert_brdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

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

/*!
  \details
  No detailed.
  */
LambertBrdf::LambertBrdf(const SampledSpectra& reflectance) noexcept :
    reflectance_{reflectance}
{
}

/*!
  \details
  No detailed.
  */
Float LambertBrdf::evalPdf(
    const Vector3* /* vin */,
    const Vector3* vout,
    const Vector3& normal,
    const WavelengthSamples& /* wavelemgths */) const noexcept
{
  constexpr Float k = zisc::invert(zisc::kPi<Float>);
  const Float cos_no = zisc::dot(normal, *vout);
  return k * cos_no;
}

/*!
  \details
  No detailed.
  */
SampledSpectra LambertBrdf::evalRadiance(
    const Vector3* /* vin */,
    const Vector3* /* vout */,
    const Vector3& /* normal */,
    const WavelengthSamples& /* wavelemgths */) const noexcept
{
  constexpr Float k = zisc::invert(zisc::kPi<Float>);
  return k * reflectance_;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> LambertBrdf::evalRadianceAndPdf(
    const Vector3* /* vin */,
    const Vector3* vout,
    const Vector3& normal,
    const WavelengthSamples& /* wavelemgths */) const noexcept
{
  constexpr Float k = zisc::invert(zisc::kPi<Float>);
  const Float cos_no = zisc::dot(normal, *vout);
  ZISC_ASSERT(zisc::isInBounds(cos_no, 0.0, 1.0), "cos_no isn't [0, 1].");
  return std::make_tuple(k * reflectance_, k * cos_no);;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledDirection, SampledSpectra> LambertBrdf::sample(
    const Vector3* /* vin */,
    const Vector3& normal,
    const WavelengthSamples& /* wavelemgths */,
    Sampler& sampler) const noexcept
{
  const auto vout = SampledDirection::sampleOnHemisphere<1>(normal, sampler);
  return std::make_tuple(std::move(vout), reflectance_);
}

/*!
  \details
  No detailed.
  */
bool LambertBrdf::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo
