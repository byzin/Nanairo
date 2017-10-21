/*!
  \file camera_model-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CAMERA_MODEL_INL_HPP
#define NANAIRO_CAMERA_MODEL_INL_HPP

#include "camera_model.hpp"
// Standard C++ library
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_pool.hpp"
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
void CameraModel::addContribution(const Index2d& index,
                                  const SampledSpectra& contribution) noexcept
{
  auto& spectra_buffer = film().spectraBuffer();
  spectra_buffer.addContribution(index, contribution);
}

/*!
  \details
  No detailed.
  */
inline
Film& CameraModel::film() noexcept
{
  ZISC_ASSERT(film_ != nullptr, "The film is null.");
  return *film_;
}

/*!
  \details
  No detailed.
  */
inline
const Film& CameraModel::film() const noexcept
{
  ZISC_ASSERT(film_ != nullptr, "The film is null.");
  return *film_;
}

/*!
  \details
  No detailed.
  */
inline
auto CameraModel::makeSensor(
    const Index2d& index,
    const WavelengthSamples& /* wavelengths */,
    zisc::MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  auto chunk = memory_pool.allocate<Sensor>();
  ShaderPointer ptr = makeUnique<Sensor>(chunk, this, index);
  return ptr;
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
  jittering_[0] = (is_jittering_enabled_) ? sampler.sample() : 0.0;
  jittering_[1] = (is_jittering_enabled_) ? sampler.sample() : 0.0;
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
void CameraModel::setFilm(Film* film) noexcept
{
  film_ = film;
  if (film != nullptr)
    initFilmShape();
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

} // namespace nanairo

#endif // NANAIRO_CAMERA_MODEL_INL_HPP
