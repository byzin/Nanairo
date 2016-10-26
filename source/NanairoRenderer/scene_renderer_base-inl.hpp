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
#include "NanairoCore/LinearAlgebra/transformation.hpp"
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
  emit stop();
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
  bool flag = false;
  if (is_power2_saving_)
    flag = (cycle & (cycle - 1)) == 0;
  return flag;
}

/*!
  \details
  No detailed.
  */
inline
bool SceneRendererBase::isLastCycle(const quint64 cycle) const noexcept
{
  return (cycle == termination_pass_);
}

/*!
  \details
  No detailed.
  */
inline
bool SceneRendererBase::isTimeToSaveImage(const Clock::duration& time,
                                          Clock::rep* interval_count) const noexcept
{
  if ((saving_interval_time_ * (*interval_count)) <= time) {
    ++(*interval_count);
    return true;
  }
  return false;
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
  using std::chrono::duration_cast;

  constexpr auto time_per_frame =
      std::chrono::milliseconds{1000 / RendererConfig::maxFps()};
  constexpr auto wait_time = duration_cast<Clock::duration>(time_per_frame);

  if (wait_time > delta_time) {
    std::this_thread::sleep_for(wait_time - delta_time);
    return true;
  }
  return false;
}

} // namespace nanairo

#endif // _NANAIRO_SCENE_RENDERER_BASE_INL_HPP_
