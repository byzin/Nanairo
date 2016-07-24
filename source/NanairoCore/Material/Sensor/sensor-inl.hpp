/*!
  \file sensor-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SENSOR_INL_HPP
#define NANAIRO_SENSOR_INL_HPP

#include "sensor.hpp"
// Standard C++ library
#include <tuple>
#include <utility>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"


namespace nanairo {

// Forward declaration
class Sampler;
template <uint> class WavelengthSamples;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
Sensor<kSampleSize>::Sensor(const CameraModel* camera,
                            const uint x,
                            const uint y) noexcept :
    camera_{camera},
    x_{x},
    y_{y}
{
  initialize(x, y);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
Float Sensor<kSampleSize>::evaluatePdf(
    const Vector3* /* vin */,
    const Vector3* vout,
    const Vector3& /* normal */,
    const Wavelengths& /* wavelengths */) const noexcept
{
  ZISC_ASSERT(vout != nullptr, "The vout is NULL.");
  const Float pdf = camera().calcPdf(*vout);
  return pdf;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto Sensor<kSampleSize>::evaluateRadiance(
    const Vector3* /* vin */,
    const Vector3* vout,
    const Vector3& /* normal */,
    const Wavelengths& wavelengths) const noexcept -> Spectra
{
  ZISC_ASSERT(vout != nullptr, "The vout is NULL.");
  const Float f = camera().calcRadiance(*vout);
  return Spectra{wavelengths, f};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto Sensor<kSampleSize>::evaluateRadianceAndPdf(
    const Vector3* /* vin */,
    const Vector3* vout,
    const Vector3& /* normal */,
    const Wavelengths& wavelengths) const noexcept -> std::tuple<Spectra, Float>
{
  ZISC_ASSERT(vout != nullptr, "The vout is NULL.");
  const auto result = camera().calcRadianceAndPdf(*vout);
  const Float f = std::get<0>(result);
  const Float pdf = std::get<1>(result);
  return std::make_tuple(Spectra{wavelengths, f}, pdf);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto Sensor<kSampleSize>::sample(
    const Vector3* /* vin */,
    const Vector3& /* normal */,
    const Wavelengths& wavelengths,
    Sampler& /* sampler */) const noexcept -> std::tuple<SampledDirection, Spectra>
{
  const auto vout = camera().sampleDirection(x_, y_);
  const auto weight = Spectra{wavelengths, 1.0};
  return std::make_tuple(vout, weight);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
bool Sensor<kSampleSize>::wavelengthIsSelected() const noexcept
{
  return false;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
const CameraModel& Sensor<kSampleSize>::camera() const noexcept
{
  return *camera_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void Sensor<kSampleSize>::initialize(const uint /* x */,
                                     const uint /* y */) noexcept
{
}

} // namespace nanairo

#endif // _NANAIRO_SENSOR_INL_HPP_
