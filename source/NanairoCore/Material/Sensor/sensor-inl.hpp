/*!
  \file sensor-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SENSOR_INL_HPP_
#define _NANAIRO_SENSOR_INL_HPP_

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
                            const uint y) :
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
    const Wavelengths& /* wavelengths */) const
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
    const Wavelengths& wavelengths) const -> Spectra
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
    const Wavelengths& wavelengths) const -> std::tuple<Spectra, Float>
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
    Sampler& /* sampler */) const -> std::tuple<SampledDirection, Spectra>
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
bool Sensor<kSampleSize>::wavelengthIsSelected() const
{
  return false;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
const CameraModel& Sensor<kSampleSize>::camera() const
{
  return *camera_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void Sensor<kSampleSize>::initialize(const uint /* x */,
                                     const uint /* y */)
{
}

} // namespace nanairo

#endif // _NANAIRO_SENSOR_INL_HPP_
