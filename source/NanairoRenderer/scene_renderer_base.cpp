/*!
  \file scene_renderer_base.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene_renderer_base.hpp"
// Standard C++ library
#include <chrono>
#include <limits>
#include <thread>
// Qt
#include <QColor>
#include <QImage>
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
#include "NanairoCore/Utility/scene_settings.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SceneRendererBase::SceneRendererBase() :
    camera_matrix_{makeIdentityMatrix()}
{
}

/*!
  \details
  No detailed.
  */
SceneRendererBase::~SceneRendererBase()
{
}

/*!
  \details
  No detailed.
  */
void SceneRendererBase::previewImage()
{
  using zisc::cast;

  // Initialize
  outputMessage(QStringLiteral("Start preview."));
  zisc::Stopwatch stopwatch;
  quint64 cycle = 0;
  auto previous_time = Clock::duration::zero();
  bool rendering_flag = true;
  auto stop_rendering = [&rendering_flag](){rendering_flag = false;};
  auto connection = connect(this, &SceneRendererBase::stop, stop_rendering);

  // Main process
  stopwatch.start();
  while (rendering_flag) {
    // Camera event
    handleCameraEvent(&cycle, &stopwatch);

    ++cycle;

    render(cycle);
    convertSpectraToHdr(cycle);
    toneMap();

    auto elapsed_time = stopwatch.elapsedTime();
    emit updated(cycle, cast<qint64>(elapsed_time.count()));

    if (waitForNextFrame(elapsed_time - previous_time))
      elapsed_time = stopwatch.elapsedTime();
    previous_time = elapsed_time;
  }
  disconnect(connection);
  outputMessage(QStringLiteral("Finish preview."));
  outputMessage(QStringLiteral("----------------------------------------"));
  emit outputMatrix(cameraMatrix());
  emit finished();
}

/*!
  \details
  No detailed.
  */
void SceneRendererBase::renderImage(const QString& output_dir)
{
  using zisc::cast;

  // Initialize
  outputMessage(QStringLiteral("Start rendering."));
  zisc::Stopwatch stopwatch;
  quint64 cycle = 0;
  Clock::rep interval_count = 1;
  auto previous_time = Clock::duration::zero();
  bool rendering_flag = true;
  auto stop_rendering = [&rendering_flag](){rendering_flag = false;};
  auto connection = connect(this, &SceneRendererBase::stop, stop_rendering);

  // Main process
  stopwatch.start();
  while (rendering_flag && !isLastCycle(cycle)) {
    ++cycle;

    render(cycle);
    if (isCycleToSaveImage(cycle) || 
        isTimeToSaveImage(previous_time, &interval_count)) {
      convertSpectraToHdr(cycle);
      toneMap();
      saveLdrImage(cycle, output_dir);
    }

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
void SceneRendererBase::outputMatrix(const Matrix4x4& matrix) const
{
  QMatrix4x4 m;
  for (int row = 0; row < 4; ++row) {
    for (int column = 0; column < 4; ++column) {
      m(row, column) = matrix(row, column);
    }
  }
  if (!m.isIdentity()) {
    m.optimize();
    outputMatrix(m);
  }
}

/*!
  \details
  No detailed.
  */
void SceneRendererBase::initialize(const SceneSettings& settings)
{
  using zisc::cast;
  using std::chrono::duration_cast;
  using Clock = zisc::Stopwatch::Clock;
  using Millis = std::chrono::milliseconds;

  outputMessage(QStringLiteral("Initialize the renderer."));

  const QString prefix{keyword::system};

  auto key = prefix + "/" + keyword::terminationPass;
  termination_pass_ = cast<quint64>(settings.intValue(key));
  termination_pass_ = (termination_pass_ == 0)
      ? std::numeric_limits<quint64>::max()
      : termination_pass_;

  key = prefix + "/" + keyword::power2Saving;
  is_power2_saving_ = settings.booleanValue(key);
  key = prefix + "/" + keyword::savingInterval;
  const int interval_time = settings.intValue(key);
  saving_interval_time_ = duration_cast<Clock::duration>(Millis{interval_time});

  key = prefix + "/" + keyword::imageWidthResolution;
  const int width = settings.intValue(key);
  key = prefix + "/" + keyword::imageHeightResolution;
  const int height = settings.intValue(key);
  ldr_image_ = QImage{QSize{width, height}, QImage::Format_RGB32};
  const QColor black{Qt::black};
  ldr_image_.fill(black);

  key = prefix + "/" + keyword::ldrImageFormat;
  ldr_image_format_ = settings.stringValue(key).toLower();

  initializeRenderer(settings);
}

} // namespace nanairo
