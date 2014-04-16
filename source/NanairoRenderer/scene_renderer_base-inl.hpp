/*!
  \file scene_renderer_base-inl.hpp
  \author Sho Ikeda
  */

#ifndef _NANAIRO_SCENE_RENDERER_BASE_INL_HPP_
#define _NANAIRO_SCENE_RENDERER_BASE_INL_HPP_

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
  \details
  No detailed.
  */
inline
void SceneRendererBase::handlePreviewMouseEvent(const int buttons,
                                                const int delta_x,
                                                const int delta_y,
                                                const int delta_wheel)
{
  camera_event_.addEvent(buttons, delta_x, delta_y, delta_wheel);
}

/*!
  \details
  No detailed.
  */
inline
void SceneRendererBase::handleCameraEvent(quint64* cycle, zisc::Stopwatch* /* stopwatch */)
{
  if (camera_event_.isEventOccured()) {
    handleCameraEvent();
    *cycle = 0;
//    stopwatch->stop();
//    stopwatch->start();
    camera_event_.clear();
  }
}

/*!
  \details
  No detailed.
  */
inline
const QString& SceneRendererBase::ldrImageFormat() const
{
  return ldr_image_format_;
}

/*!
  \details
  No detailed.
  */
inline
QImage& SceneRendererBase::renderedImage()
{
  return ldr_image_;
}

/*!
  \details
  No detailed.
  */
inline
const QImage& SceneRendererBase::renderedImage() const
{
  return ldr_image_;
}

/*!
  \details
  No detailed.
  */
inline
void SceneRendererBase::stopRendering() const
{
  emit stop();
}

/*!
  \details
  No detailed.
  */
inline
const CameraEvent& SceneRendererBase::cameraEvent() const
{
  return camera_event_;
}

/*!
  \details
  No detailed.
  */
inline
const Matrix4x4& SceneRendererBase::cameraMatrix() const
{
  return camera_matrix_;
}

/*!
  \details
  No detailed.
  */
inline
Matrix4x4& SceneRendererBase::cameraMatrix()
{
  return camera_matrix_;
}

/*!
  \details
  No detailed.
  */
inline
bool SceneRendererBase::isCycleToSaveImage(const quint64 cycle) const
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
bool SceneRendererBase::isLastCycle(const quint64 cycle) const
{
  return (cycle == termination_pass_);
}

/*!
  \details
  No detailed.
  */
inline
bool SceneRendererBase::isTimeToSaveImage(const Clock::duration& time,
                                          Clock::rep* interval_count) const
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
                                     const QString& output_dir) const
{
  const auto file_name = output_dir + "/" +
                         QString::number(cycle) + "cycle." + ldrImageFormat();
  ldr_image_.save(file_name);
}

/*!
  \details
  No detailed.
  */
inline
bool SceneRendererBase::waitForNextFrame(const Clock::duration& delta_time) const
{
  using std::chrono::duration_cast;

  constexpr auto time_per_frame = std::chrono::milliseconds{1000 / kMaxFps};
  constexpr auto wait_time = duration_cast<Clock::duration>(time_per_frame);

  if (wait_time > delta_time) {
    std::this_thread::sleep_for(wait_time - delta_time);
    return true;
  }
  return false;
}

} // namespace nanairo

#endif // _NANAIRO_SCENE_RENDERER_BASE_INL_HPP_
