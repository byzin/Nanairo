/*!
  \file scene_renderer_base.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene_renderer_base.hpp"
// Standard C++ library
#include <chrono>
#include <limits>
#include <thread>
#include <type_traits>
// Qt
#include <QColor>
#include <QImage>
#include <QMatrix4x4>
#include <QObject>
#include <QString>
#include <QtGlobal>
// Zisc
#include "zisc/stopwatch.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "scene_value.hpp"
#include "NanairoCore/Color/ldr_image.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Setting/scene_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/system_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SceneRendererBase::SceneRendererBase() noexcept
{
}

/*!
  \details
  No detailed.
  */
SceneRendererBase::~SceneRendererBase() noexcept
{
}

/*!
  \details
  No detailed.
  */
void SceneRendererBase::initialize(const SettingNodeBase* settings) noexcept
{
  using zisc::cast;
  using std::chrono::duration_cast;
  using Clock = zisc::Stopwatch::Clock;
  using Millis = std::chrono::milliseconds;

  qInfo("Initialize the renderer.");

  const auto scene_settings = castNode<SceneSettingNode>(settings);
  const auto system_settings =
      castNode<SystemSettingNode>(scene_settings->systemSettingNode());
  // Termination cycle
  {
    auto termination_cycle = cast<quint64>(system_settings->terminationCycle());
    termination_cycle = (termination_cycle == 0)
        ? std::numeric_limits<quint64>::max()
        : termination_cycle;
    termination_cycle_ = termination_cycle;
    qInfo("  Termination cycle: %llu", termination_cycle_);
  }
  // Power2 cycle saving
  {
    is_power2_cycle_saving_ = system_settings->power2CycleSaving();
    const auto interval_time = system_settings->savingIntervalTime();
    saving_interval_time_ = duration_cast<Clock::duration>(Millis{interval_time});
    qInfo("  Saving2 cycle mode: %d", is_power2_cycle_saving_);
  }
  // LDR image
  {
    const auto& image_resolution = system_settings->imageResolution();
    ldr_image_impl_ = new LdrImage{image_resolution[0], image_resolution[1]};

    ldr_image_ = QImage{zisc::treatAs<uint8*>(&(ldr_image_impl_->data()[0])), 
                        cast<int>(image_resolution[0]),
                        cast<int>(image_resolution[1]),
                        QImage::Format_RGB32};
    qInfo("  Image resolution: %d x %d", cast<int>(image_resolution[0]), cast<int>(image_resolution[1]));
  }

  cameraMatrix() = Transformation::makeIdentity();

  initializeRenderer(settings);
}

/*!
  \details
  No detailed.
  */
void SceneRendererBase::previewImage() noexcept
{
  using zisc::cast;

  // Initialize
  qInfo("Start preview rendering.");
  // Renderer stopwatch
  zisc::Stopwatch stopwatch;
  quint64 cycle = 0;
  auto previous_time = Clock::duration::zero();
  // Rendering flag
  bool is_runnable = true;
  auto stop_rendering = [&is_runnable]() noexcept {is_runnable = false;};
  auto connection = connect(this, &SceneRendererBase::stopping, stop_rendering);

  // Main process
  stopwatch.start();
  while (is_runnable && !isLastCycle(cycle)) {
    // Camera event
    handleCameraEvent(&cycle, &stopwatch);

    // Rendering
    ++cycle;
    render(cycle);
    // Buffer to image
    convertSpectraToHdr(cycle);
    toneMap();

    // Update time
    const auto elapsed_time = stopwatch.elapsedTime();
    emit updated(cycle, cast<qint64>(elapsed_time.count()));
    previous_time = waitForNextFrame(elapsed_time - previous_time)
        ? stopwatch.elapsedTime()
        : elapsed_time;
  }
  disconnect(connection);
  qInfo("Finish preview rendering.");
  if (cameraMatrix() != Transformation::makeIdentity())
    outputCameraEvent();
  emit finished();
}

/*!
  \details
  No detailed.
  */
void SceneRendererBase::renderImage(const QString& output_dir) noexcept
{
  using zisc::cast;

  // Initialize
  qInfo("Start rendering.");
  // Renderer stopwatch
  zisc::Stopwatch stopwatch;
  quint64 cycle = 0;
  Clock::rep interval_count = 1;
  auto previous_time = Clock::duration::zero();
  // Rendering flag
  bool is_runnable = true;
  auto stop_rendering = [&is_runnable]() noexcept {is_runnable = false;};
  auto connection = connect(this, &SceneRendererBase::stopping, stop_rendering);

  // Main process
  stopwatch.start();
  while (is_runnable && !isLastCycle(cycle)) {
    // Rendering
    ++cycle;
    render(cycle);
    // Save image
    if (isCycleToSaveImage(cycle) ||
        isTimeToSaveImage(previous_time, &interval_count)) {
      convertSpectraToHdr(cycle);
      toneMap();
      saveLdrImage(cycle, output_dir);
    }
    // Update time
    const auto elapsed_time = stopwatch.elapsedTime();
    emit updated(cycle, cast<qint64>(elapsed_time.count()));
    previous_time = waitForNextFrame(elapsed_time - previous_time)
        ? stopwatch.elapsedTime()
        : elapsed_time;
  }
  disconnect(connection);
  qInfo("Finish rendering.");
  emit finished();
}

/*!
  \details
  No detailed.
  */
inline
void SceneRendererBase::outputCameraEvent() const noexcept
{
  const auto& camera_matrix = cameraMatrix();
  QMatrix4x4 matrix;
  for (uint row = 0; row < Matrix4x4::rowSize(); ++row) {
    for (uint column = 0; column < Matrix4x4::columnSize(); ++column) {
      matrix(row, column) = zisc::cast<float>(camera_matrix(row, column));
    }
  }
  emit cameraEventHandled(matrix);
}

} // namespace nanairo
