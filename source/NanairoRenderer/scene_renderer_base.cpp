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
#include "NanairoCore/LinearAlgebra/transformation.hpp"

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

  outputMessage(QStringLiteral("Initialize the renderer."));

  auto system_node = SceneValue::toObject(settings, keyword::system);

  termination_pass_ = SceneValue::toInt<qint64>(system_node,
                                                keyword::terminationCycle);
  termination_pass_ = (termination_pass_ == 0)
      ? std::numeric_limits<quint64>::max()
      : termination_pass_;

  is_power2_saving_ = SceneValue::toBool(system_node, keyword::power2CycleSaving);
  const int interval_time = SceneValue::toInt<int>(system_node, keyword::savingInterval);
  saving_interval_time_ = duration_cast<Clock::duration>(Millis{interval_time});

  const auto image_resolution = SceneValue::toArray(system_node,
                                                    keyword::imageResolution);
  const int width = SceneValue::toInt<int>(image_resolution[0]);
  const int height = SceneValue::toInt<int>(image_resolution[1]);
  ldr_image_ = QImage{QSize{width, height}, QImage::Format_RGB32};
  const QColor black{Qt::black};
  ldr_image_.fill(black);

  cameraMatrix() = makeIdentityMatrix();

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
  outputMessage(QStringLiteral("Start preview."));
  // Renderer stopwatch
  zisc::Stopwatch stopwatch;
  quint64 cycle = 0;
  auto previous_time = Clock::duration::zero();
  // Rendering flag
  bool rendering_flag = true;
  auto stop_rendering = [&rendering_flag]() noexcept {rendering_flag = false;};
  auto connection = connect(this, &SceneRendererBase::stop, stop_rendering);

  // Main process
  stopwatch.start();
  while (rendering_flag) {
    // Camera event
    handleCameraEvent(&cycle, &stopwatch);

    ++cycle;

    // Rendering
    render(cycle);
    // Buffer to image
    convertSpectraToHdr(cycle);
    toneMap();

    // Update time
    auto elapsed_time = stopwatch.elapsedTime();
    emit updated(cycle, cast<qint64>(elapsed_time.count()));
    if (waitForNextFrame(elapsed_time - previous_time))
      elapsed_time = stopwatch.elapsedTime();
    previous_time = elapsed_time;
  }
  disconnect(connection);
  outputMessage(QStringLiteral("Finish preview."));
  outputMessage(QStringLiteral("----------------------------------------"));
  if (cameraMatrix() != makeIdentityMatrix())
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
  outputMessage(QStringLiteral("Start rendering."));
  // Renderer stopwatch
  zisc::Stopwatch stopwatch;
  quint64 cycle = 0;
  Clock::rep interval_count = 1;
  auto previous_time = Clock::duration::zero();
  // Rendering flag
  bool rendering_flag = true;
  auto stop_rendering = [&rendering_flag]() noexcept {rendering_flag = false;};
  auto connection = connect(this, &SceneRendererBase::stop, stop_rendering);

  // Main process
  stopwatch.start();
  while (rendering_flag && !isLastCycle(cycle)) {
    ++cycle;
    // Rendering
    render(cycle);
    // Save image
    if (isCycleToSaveImage(cycle) || 
        isTimeToSaveImage(previous_time, &interval_count)) {
      convertSpectraToHdr(cycle);
      toneMap();
      saveLdrImage(cycle, output_dir);
    }
    // Update time
    auto elapsed_time = stopwatch.elapsedTime();
    emit updated(cycle, cast<qint64>(elapsed_time.count()));
    if (waitForNextFrame(elapsed_time - previous_time))
      elapsed_time = stopwatch.elapsedTime();
    previous_time = elapsed_time;
  }
  disconnect(connection);
  outputMessage(QStringLiteral("Finish rendering."));
  outputMessage(QStringLiteral("----------------------------------------"));
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
