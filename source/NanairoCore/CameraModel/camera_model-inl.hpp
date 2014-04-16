/*!
  \file camera_model-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_CAMERA_MODEL_INL_HPP_
#define _NANAIRO_CAMERA_MODEL_INL_HPP_

#include "camera_model.hpp"
// Standard C++ library
#include <utility>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
// Nanairo
#include "film.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Color/spectra_image_interface.hpp"
#include "NanairoCore/Material/Sensor/sensor.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void CameraModel::addContribution(const uint x, 
                                  const uint y, 
                                  const SampledSpectra<kSampleSize>& contribution)
{
  auto& spectra_buffer = film().spectraBuffer();
  spectra_buffer.addContribution(x, y, contribution);
}

/*!
  \details
  No detailed.
  */
inline
Film& CameraModel::film()
{
  return *film_;
}

/*!
  \details
  No detailed.
  */
inline
const Film& CameraModel::film() const
{
  return *film_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto CameraModel::makeSensor(
    const uint x,
    const uint y,
    const WavelengthSamples<kSampleSize>& /* wavelengths */,
    MemoryPool& memory_pool) const -> ShaderPointer<kSampleSize>
{
  auto sensor = memory_pool.allocate<Sensor<kSampleSize>>(this, x, y);
  return ShaderPointer<kSampleSize>{sensor};
}

/*!
  \details
  No detailed.
  */
inline
const Vector3& CameraModel::normal() const
{
  return normal_;
}

/*!
  \details
  No detailed.
  */
inline
uint CameraModel::heightResolution() const
{
  return film().heightResolution();
}

/*!
  \details
  No detailed.
  */
inline
void CameraModel::jitter(Sampler& sampler)
{
  if (is_jittering_enabled_) {
    jittering_[0] = sampler.sample(0.0, 1.0);
    jittering_[1] = sampler.sample(0.0, 1.0);
  }
  else {
    jittering_[0] = 0.0;
    jittering_[1] = 0.0;
  }
}

/*!
  \details
  No detailed.
  */
inline
const Vector2& CameraModel::jittering() const
{
  return jittering_;
}

/*!
  \details
  No detailed.
  */
inline
const Point3& CameraModel::sampledLensPoint() const
{
  return sampled_lens_point_;
}

/*!
  \details
  No detailed.
  */
inline
void CameraModel::setFilm(Film* film)
{
  film_ = film;
  initializeFilm();
}

/*!
  \details
  No detailed.
  */
inline
uint CameraModel::widthResolution() const
{
  return film().widthResolution();
}

/*!
  \details
  No detailed.
  */
inline
void CameraModel::setNormal(const Vector3& normal)
{
  normal_ = normal;
}

/*!
  \details
  No detailed.
  */
inline
void CameraModel::setSampledLensPoint(const Point3& sampled_point)
{
  sampled_lens_point_ = sampled_point;
}

} // namespace nanairo

#endif // _NANAIRO_CAMERA_MODEL_INL_HPP_
