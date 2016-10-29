/*!
  \file scene_renderer_base-inl.hpp
  \author Sho Ikeda
  */

#ifndef NANAIRO_SCENE_RENDERER_BASE_INL_HPP
#define NANAIRO_SCENE_RENDERER_BASE_INL_HPP

#include "scene_renderer_base.hpp"
// Standard C++ library
#include <chrono>
#include <thread>
// Qt
#include <QImage>
#include <QString>
#include <QtGlobal>
// ZIsc
#include "zisc/stopwatch.hpp"
// Nanairo
#include "camera_event.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoRenderer/nanairo_renderer_config.hpp"

namespace nanairo {

/*!
  */
inline
Matrix4x4& SceneRendererBase::cameraMatrix() noexcept
{
  return camera_matrix_;
}

/*!
  */
inline
const Matrix4x4& SceneRendererBase::cameraMatrix() const noexcept
{
  return camera_matrix_;
}

/*!
  \details
  No detailed.
  */
inline
void SceneRendererBase::handlePreviewEvent(const int transformation_event_type,
                                           const int axis_event_type,
                                           const int value) noexcept
{
  camera_event_.addEvent(transformation_event_type, axis_event_type, value);
}

/*!
  \details
  No detailed.
  */
inline
void SceneRendererBase::handleCameraEvent(quint64* cycle,
                                          zisc::Stopwatch* /* stopwatch */)
    noexcept
{
  if (camera_event_.isEventOccured()) {
    handleCameraEvent();
    //! \todo Very slow
//    stopwatch->stop();
//    stopwatch->start();
    *cycle = 0;
  }
}

/*!
  \details
  No detailed.
  */
inline
QImage& SceneRendererBase::renderedImage() noexcept
{
  return ldr_image_;
}

/*!
  \details
  No detailed.
  */
inline
const QImage& SceneRendererBase::renderedImage() const noexcept
{
  return ldr_image_;
}

/*!
  \details
  No detailed.
  */
inline
void SceneRendererBase::stopRendering() const noexcept
{
  emit stopping();
}

/*!
  \details
  No detailed.
  */
inline
CameraEvent& SceneRendererBase::cameraEvent() noexcept
{
  return camera_event_;
}

/*!
  \details
  No detailed.
  */
inline
const CameraEvent& SceneRendererBase::cameraEvent() const noexcept
{
  return camera_event_;
}

/*!
  \details
  No detailed.
  */
inline
bool SceneRendererBase::isCycleToSaveImage(const quint64 cycle) const noexcept
{
  const bool flag = (is_power2_cycle_saving_)
      ? (cycle & (cycle - 1)) == 0
      : false;
  return flag;
}

/*!
  \details
  No detailed.
  */
inline
bool SceneRendererBase::isLastCycle(const quint64 cycle) const noexcept
{
  return (cycle == termination_cycle_);
}

/*!
  \details
  No detailed.
  */
inline
bool SceneRendererBase::isTimeToSaveImage(const Clock::duration& time,
                                          Clock::rep* interval_count) const noexcept
{
  const bool is_time_to_save = (saving_interval_time_ * (*interval_count)) <= time;
  if (is_time_to_save)
    ++(*interval_count);
  return is_time_to_save;
}

/*!
  \details
  No detailed.
  */
inline
void SceneRendererBase::saveLdrImage(const quint64 cycle, 
                                     const QString& output_dir) const noexcept
{
  const auto file_name = output_dir + "/" + QString::number(cycle) + "cycle.png";
  ldr_image_.save(file_name);
}

/*!
  \details
  No detailed.
  */
inline
bool SceneRendererBase::waitForNextFrame(const Clock::duration& delta_time) const noexcept
{
  using Nano = std::chrono::nanoseconds;
  using std::chrono::duration_cast;
  constexpr auto nano_time_per_frame = Nano{1'000'000'000 / RendererConfig::maxFps()};
  constexpr auto time_per_frame = duration_cast<Clock::duration>(nano_time_per_frame);

  const bool is_faster = delta_time < time_per_frame;
  if (is_faster)
    std::this_thread::sleep_for(time_per_frame - delta_time);
  return is_faster;
}

} // namespace nanairo

#endif // _NANAIRO_SCENE_RENDERER_BASE_INL_HPP_
