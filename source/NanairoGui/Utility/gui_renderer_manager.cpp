/*!
  \file gui_renderer_manager.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_renderer_manager.hpp"
// Standard C++ library
#include <cstdint>
#include <functional>
#include <utility>
// Qt
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QJsonObject>
#include <QMatrix4x4>
#include <QObject>
#include <QSizeF>
#include <QSharedPointer>
#include <QString>
#include <QThread>
#include <QUrl>
#include <QVariant>
#include <QtGlobal>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/math.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/stopwatch.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "rendered_image_provider.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoGui/nanairo_gui_config.hpp"
#include "NanairoRenderer/cpu_scene_renderer.hpp"
#include "NanairoRenderer/renderer_utility.hpp"
#include "NanairoRenderer/scene_document.hpp"
#include "NanairoRenderer/scene_renderer_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
GuiRendererManager::GuiRendererManager() noexcept :
    rendering_thread_{1},
    image_provider_{nullptr}
{
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::outputCameraEvent(const QMatrix4x4& matrix) const noexcept
{
  emit cameraEventHandled(matrix);
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::finishRendering() noexcept
{
  setRenderingInfo(0, 1);
  emit finished();
}

/*!
  \details
  No detailed.
  */
QString GuiRendererManager::currentWorkingDir() const noexcept
{
  return QDir{}.absolutePath();
}

/*!
  \details
  No detailed.
  */
QString GuiRendererManager::getFileName(const QUrl& file_path) const noexcept
{
  return file_path.fileName();
}

/*!
  \details
  No detailed.
  */
int GuiRendererManager::idealThreadCount() const noexcept
{
  return QThread::idealThreadCount();
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::invokeRendering(const QString& output_dir) noexcept
{
  // Load a scene data
  const QString scene_file_path = output_dir + "/" + keyword::sceneBackupFileName;
  QJsonObject settings;
  QString message;
  const bool result = SceneDocument::loadDocument(scene_file_path, settings, message);
  if (!result) {
    qFatal("%s", message.toStdString().c_str());
  }

  // Invoke renderer
  QSharedPointer<SceneRendererBase> renderer{new CpuSceneRenderer};
  setRenderer(renderer.data());
  auto render = [this, renderer, output_dir, settings]()
  {
    // Initialize the renderer
    renderer->initialize(settings);
    // Start rendering
    emit started();
    renderer->renderImage(output_dir);
    // Finish rendering
    image_provider_->setImage(nullptr);
  };
  rendering_thread_.enqueue<void>(render);
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::invokePreviewing() noexcept
{
  // Load a scene data
  const QString scene_file_path = QString{keyword::previewDir} + "/" +
                                  keyword::sceneBackupFileName;
  QJsonObject settings;
  QString message;
  const bool result = SceneDocument::loadDocument(scene_file_path, settings, message);
  if (!result) {
    qFatal("%s", message.toStdString().c_str());
  }

  // Invoke renderer
  QSharedPointer<SceneRendererBase> renderer{new CpuSceneRenderer};
  setRenderer(renderer.data());
  auto preview = [this, renderer, settings]()
  {
    // Initialize the renderer
    renderer->initialize(settings);
    // Start rendering
    emit started();
    renderer->previewImage();
    // Finish rendering
    image_provider_->setImage(nullptr);
  };
  rendering_thread_.enqueue<void>(preview);
}

/*!
  */
bool GuiRendererManager::isDebugMode() const noexcept
{
  return false;
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::makeDir(const QString& dir) const noexcept
{
  const QDir current_dir{};
  current_dir.mkdir(dir);
}

/*!
  \details
  No detailed.
  */
int GuiRendererManager::random() const noexcept
{
  return qrand();
}

/*!
  */
QVariantMap GuiRendererManager::loadSceneData(const QUrl& file_url) const noexcept
{
  QVariantMap scene_data;
  QString message;
  const bool result = SceneDocument::loadDocument(file_url, scene_data, message);
  if (!result) {
    qFatal("%s", message.toStdString().c_str());
  }
  return scene_data;
}

/*!
  */
void GuiRendererManager::saveSceneData(const QUrl& file_url,
                                       const QVariantMap& scene_data) const noexcept
{
  QString message;
  const bool result = SceneDocument::saveDocument(file_url, scene_data, message);
  if (!result) {
    qFatal("%s", message.toStdString().c_str());
  }
}

/*!
  */
QUrl GuiRendererManager::toAbsoluteFileUrl( const QString& relative_path) const noexcept
{
  QDir working_dir;
  const auto absolute_path = working_dir.absolutePath() + "/" +
                             QDir::cleanPath(relative_path);
  return QUrl::fromLocalFile(absolute_path);
}

/*!
  */
QString GuiRendererManager::toRelativeFilePath(
    const QUrl& absolute_path) const noexcept
{
  const QDir working_dir;
  return QDir::cleanPath(working_dir.relativeFilePath(absolute_path.toLocalFile()));
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::setRenderedImageProvider(
    RenderedImageProvider* image_provider) noexcept
{
  image_provider_ = image_provider;
  image_provider_->setImage(nullptr);
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::setRenderer(const SceneRendererBase* renderer) noexcept
{
  connect(renderer, &SceneRendererBase::updated,
          this, &GuiRendererManager::setRenderingInfo);
  connect(renderer, &SceneRendererBase::finished,
          this, &GuiRendererManager::finishRendering);
  connect(renderer, &SceneRendererBase::cameraEventHandled,
          this, &GuiRendererManager::outputCameraEvent);
  connect(this, &GuiRendererManager::stopping,
          renderer, &SceneRendererBase::stopRendering);
  connect(this, &GuiRendererManager::previewEvent,
          renderer, &SceneRendererBase::handlePreviewEvent);

  image_provider_->setImage(&renderer->renderedImage());
}

/*!
  \details
  No detailed.
  */
void GuiRendererManager::setRenderingInfo(const quint64 cycle,
                                          const qint64 time) noexcept
{
  using zisc::cast;
  constexpr quint64 k = cast<quint64>(zisc::Stopwatch::Clock::period::den);
  const double fps = cast<double>(k * cycle) / cast<double>(time);
  const auto fps_string = QString::number(fps, 'f', 2);
  const auto cycle_string = QStringLiteral("%1").arg(cycle, 8, 10, QChar('0'));
  const auto time_string = getTimeString(zisc::Stopwatch::Clock::duration{time});

  emit updated(fps_string, cycle_string, time_string);
}

} // namespace nanairo
