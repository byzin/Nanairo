/*!
  \file camera_model-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CAMERA_MODEL_INL_HPP
#define NANAIRO_CAMERA_MODEL_INL_HPP

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
inline
void CameraModel::addContribution(const uint x,
                                  const uint y,
                                  const SampledSpectra& contribution) noexcept
{
  auto& spectra_buffer = film().spectraBuffer();
  spectra_buffer.addContribution(x, y, contribution);
}

/*!
  \details
  No detailed.
  */
inline
Film& CameraModel::film() noexcept
{
  return *film_;
}

/*!
  \details
  No detailed.
  */
inline
const Film& CameraModel::film() const noexcept
{
  return *film_;
}

/*!
  \details
  No detailed.
  */
inline
auto CameraModel::makeSensor(
    const uint x,
    const uint y,
    const WavelengthSamples& /* wavelengths */,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  auto sensor = memory_pool.allocate<Sensor>(this, x, y);
  return ShaderPointer{sensor};
}

/*!
  \details
  No detailed.
  */
inline
const Vector3& CameraModel::normal() const noexcept
{
  return normal_;
}

/*!
  \details
  No detailed.
  */
inline
uint CameraModel::heightResolution() const noexcept
{
  return film().heightResolution();
}

/*!
  \details
  No detailed.
  */
inline
void CameraModel::jitter(Sampler& sampler) noexcept
{
  if (is_jittering_enabled_) {
    jittering_[0] = sampler.sample();
    jittering_[1] = sampler.sample();
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
const Vector2& CameraModel::jittering() const noexcept
{
  return jittering_;
}

/*!
  \details
  No detailed.
  */
inline
const Point3& CameraModel::sampledLensPoint() const noexcept
{
  return sampled_lens_point_;
}

/*!
  \details
  No detailed.
  */
inline
void CameraModel::setFilm(Film* film) noexcept
{
  film_ = film;
  initializeFilm();
}

/*!
  \details
  No detailed.
  */
inline
uint CameraModel::widthResolution() const noexcept
{
  return film().widthResolution();
}

/*!
  \details
  No detailed.
  */
inline
void CameraModel::setNormal(const Vector3& normal) noexcept
{
  normal_ = normal;
}

/*!
  \details
  No detailed.
  */
inline
void CameraModel::setSampledLensPoint(const Point3& sampled_point) noexcept
{
  sampled_lens_point_ = sampled_point;
}

} // namespace nanairo

#endif // NANAIRO_CAMERA_MODEL_INL_HPP
