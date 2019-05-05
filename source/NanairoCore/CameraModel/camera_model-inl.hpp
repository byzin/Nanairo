/*!
  \file camera_model-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "film.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/path_state.hpp"
#include "NanairoCore/Material/Sensor/sensor.hpp"
#include "NanairoCore/Sampling/sample_statistics.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"
#include "NanairoCore/Sampling/Sampler/sampler.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
void CameraModel::addContribution(const Index2d index,
                                  const SampledSpectra& contribution) noexcept
{
  auto& sample_statistics  = film().sampleStatistics();
  sample_statistics.addSample(index, contribution);
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
  */
inline
Index2d CameraModel::imageResolution() const noexcept
{
  return film().imageResolution();
}

/*!
  \details
  No detailed.
  */
inline
auto CameraModel::makeSensor(
    const Index2d& index,
    const WavelengthSamples& /* wavelengths */,
    zisc::pmr::memory_resource* mem_resource) const noexcept -> ShaderPointer
{
  using SensorPointer = zisc::UniqueMemoryPointer<Sensor>;
  auto ptr = SensorPointer::make(mem_resource, this, index);
  return std::move(ptr);
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
void CameraModel::jitter(Sampler& sampler, const PathState& path_state) noexcept
{
  if (is_jittering_enabled_) {
    const auto r = sampler.draw2D(path_state);
    jittering_[0] = r[0];
    jittering_[1] = r[1];
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
