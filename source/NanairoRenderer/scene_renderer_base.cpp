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
#include <QJsonArray>
#include <QJsonObject>
#include <QMatrix4x4>
#include <QObject>
#include <QSize>
#include <QString>
#include <QtGlobal>
// Zisc
#include "zisc/stopwatch.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

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
void SceneRendererBase::initialize(const QJsonObject& settings) noexcept
{
  using zisc::cast;
  using std::chrono::duration_cast;
  using Clock = zisc::Stopwatch::Clock;
  using Millis = std::chrono::milliseconds;

  qInfo("Initialize the renderer.");

  auto system_settings = SceneValue::toObject(settings, keyword::system);

  // Termination cycle
  {
    termination_cycle_ = SceneValue::toInt<qint64>(system_settings,
                                                   keyword::terminationCycle);
    termination_cycle_ = (termination_cycle_ == 0)
        ? std::numeric_limits<quint64>::max()
        : termination_cycle_;
    qInfo("  Termination cycle: %llu", termination_cycle_);
  }

  // Power2 cycle saving
  {
    is_power2_cycle_saving_ = SceneValue::toBool(system_settings,
                                                 keyword::power2CycleSaving);
    const int interval_time = SceneValue::toInt<int>(system_settings,
                                                     keyword::savingInterval);
    saving_interval_time_ = duration_cast<Clock::duration>(Millis{interval_time});
    qInfo("  Saving2 cycle mode: %d", is_power2_cycle_saving_);
  }

  // LDR image
  {
    const auto image_resolution = SceneValue::toArray(system_settings,
                                                      keyword::imageResolution);
    const int width = SceneValue::toInt<int>(image_resolution[0]);
    const int height = SceneValue::toInt<int>(image_resolution[1]);
    ldr_image_ = QImage{QSize{width, height}, QImage::Format_RGB32};
    const QColor black{Qt::black};
    ldr_image_.fill(black);
    qInfo("  Image resolution: %d x %d", width, height);
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
