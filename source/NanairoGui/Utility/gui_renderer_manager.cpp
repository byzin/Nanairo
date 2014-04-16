/*!
  \file gui_renderer_manager.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_renderer_manager.hpp"
// Standard C++ library
#include <cstdint>
#include <functional>
#include <utility>
// Qt
#include <QDir>
#include <QImage>
#include <QMatrix4x4>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QThread>
#include <QUrl>
#include <QtGlobal>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/math.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/stopwatch.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "rendered_image_provider.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"
#include "NanairoGui/nanairo_gui_config.hpp"
#include "NanairoRenderer/cpu_scene_renderer.hpp"
#include "NanairoRenderer/renderer_utility.hpp"
#include "NanairoRenderer/scene_renderer_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
GuiRendererManager::GuiRendererManager() :
    rendering_thread_{1},
    image_provider_{nullptr}
{
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::addCameraMatrix(const QMatrix4x4& matrix)
{
  outputCameraMatrix(matrix);
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::finishRendering()
{
  emit updated("0", "00000000", "00 h 00 m 00.000 s");
  emit finished();
}

/*!
  \details
  No detailed.
  */
QString GuiRendererManager::currentWorkingDir() const
{
  return QDir{}.absolutePath();
}

/*!
  \details
  No detailed.
  */
int GuiRendererManager::defaultRandomSeed() const
{
  using zisc::abs;
  using zisc::cast;
  constexpr int random_seed = abs(cast<int>(zisc::toHash32(kRandomSeedKey)));
  return random_seed;
}

/*!
  \details
  No detailed.
  */
QString GuiRendererManager::getFileName(const QUrl& file_path) const
{
  return file_path.fileName();
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::render(const QString& output_dir)
{
  QSharedPointer<SceneRendererBase> renderer{new CpuSceneRenderer};
  setRenderer(renderer.data());
  std::function<void ()> render{[this, renderer, output_dir]()
  {
    // Initialize the renderer
    renderer->initialize(*settings_);
    // Start rendering
    emit started();
    renderer->renderImage(output_dir);
    // Finish rendering
    image_provider_->setImage(nullptr);
  }};
  rendering_thread_.enqueue(std::move(render));
}

/*!
  \details
  No detailed.
  */
int GuiRendererManager::idealThreadCount() const
{
  return QThread::idealThreadCount();
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::makeDir(const QString& dir) const
{
  const QDir current_dir{};
  current_dir.mkdir(dir);
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::preview()
{
  QSharedPointer<SceneRendererBase> renderer{new CpuSceneRenderer};
  setRenderer(renderer.data());
  std::function<void ()> preview_rendering{[this, renderer]()
  {
    // Initialize the renderer
    renderer->initialize(*settings_);
    // Start rendering
    emit started();
    renderer->previewImage();
    // Finish rendering
    image_provider_->setImage(nullptr);
  }};
  rendering_thread_.enqueue(std::move(preview_rendering));
}

/*!
  \details
  No detailed.
  */
int GuiRendererManager::random() const
{
  return qrand();
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::setRenderedImageProvider(
    RenderedImageProvider* image_provider)
{
  image_provider_ = image_provider;
  image_provider_->setImage(nullptr);
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::setRenderer(const SceneRendererBase* renderer)
{
  connect(renderer, SIGNAL(updated(quint64, qint64)),
          this, SLOT(setRenderingInfo(quint64, qint64)));
  connect(renderer, SIGNAL(finished()), 
          this, SLOT(finishRendering()));
  connect(renderer, SIGNAL(outputMessage(const QString&)),
          this, SLOT(setMessage(const QString&)));
  connect(renderer, SIGNAL(outputMatrix(const QMatrix4x4&)),
          this, SLOT(addCameraMatrix(const QMatrix4x4&)));
  connect(this, SIGNAL(stopRendering()),
          renderer, SLOT(stopRendering()));
  connect(this, SIGNAL(previewMouseEvent(int, int, int, int)),
          renderer, SLOT(handlePreviewMouseEvent(int, int, int, int)));
          
  image_provider_->setImage(&renderer->renderedImage());
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::setRenderingInfo(const quint64 cycle, const qint64 time)
{
  using zisc::cast;
  constexpr quint64 k = cast<quint64>(zisc::Stopwatch::Clock::period::den);
  const double fps = cast<double>(k * cycle) / cast<double>(time);
  const auto fps_string = QString::number(fps, 'f', 2);
  const auto cycle_string = QStringLiteral("%1").arg(cycle, 8, 10, QChar('0'));
  const auto time_string = getTimeString(zisc::Stopwatch::Clock::duration{time});

  emit updated(fps_string, cycle_string, time_string);
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::setSceneSettings(const SceneSettings* settings)
{
  settings_ = settings;
}

} // namespace nanairo
