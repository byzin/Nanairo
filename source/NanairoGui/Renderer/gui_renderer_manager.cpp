/*!
  \file gui_renderer_manager.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_renderer_manager.hpp"
// Standard C++ library
#include <cstdint>
#include <utility>
// Qt
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QJsonObject>
#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>
#include <QThread>
#include <QUrl>
#include <QVariant>
#include <QtGlobal>
// Zisc
#include "zisc/error.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "gui_renderer.hpp"
#include "scene_document.hpp"
#include "NanairoGui/keyword.hpp"
#include "NanairoGui/rendered_image_provider.hpp"

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
QString GuiRendererManager::currentWorkingDir() const noexcept
{
  return QDir{}.absolutePath();
}

/*!
  */
int GuiRendererManager::generateSeedRandomly() const noexcept
{
  return qrand();
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
  */
int GuiRendererManager::getIdealThreadCount() const noexcept
{
  return QThread::idealThreadCount();
}

/*!
  */
QUrl GuiRendererManager::getAbsolutePath(
    const QString& relative_path) const noexcept
{
  QDir working_dir;
  const auto absolute_path = working_dir.absolutePath() + "/" +
                             QDir::cleanPath(relative_path);
  return QUrl::fromLocalFile(absolute_path);
}

/*!
  */
QString GuiRendererManager::getRelativePath(
    const QUrl& absolute_path) const noexcept
{
  const QDir working_dir;
  return QDir::cleanPath(working_dir.relativeFilePath(absolute_path.toLocalFile()));
}

/*!
  */
bool GuiRendererManager::isDebugMode() const noexcept
{
  constexpr bool is_debug_mode =
#ifdef Z_RELEASE_MODE
    false;
#else // Z_RELEASE_MODE
    true;
#endif // Z_RELEASE_MODE
  return is_debug_mode;
}

/*!
  */
void GuiRendererManager::invokeRendering(const QVariant& scene_data,
                                         const bool is_previewing) noexcept
{
  auto scene_value = QSharedPointer<QJsonObject>::create(
      QJsonObject::fromVariantMap(scene_data.toMap()));
  auto render = [this, scene_value, is_previewing]()
  {
    const GuiRenderer::RenderingMode mode = (is_previewing)
        ? GuiRenderer::RenderingMode::kPreviewing
        : GuiRenderer::RenderingMode::kRendering;
    QScopedPointer<GuiRenderer> renderer{new GuiRenderer{mode}};
    QString output_dir;
    QString error_message;
    // Prepare for rendering
    prepareForRendering(*scene_value, renderer.data(), &output_dir, &error_message);
    if (renderer->isRunnable()) {
      // Start rendering
      connectWithRenderer(renderer.data());
      emit started();
      renderer->render(output_dir.toStdString());
      // Finish rendering
      disconnectFromRenderer();
      emit finished();
    }
    else {
      QTextStream{stderr} << "Error: " << error_message;
    }
  };
  rendering_thread_.enqueue<void>(std::move(render));
}

/*!
  */
QVariant GuiRendererManager::loadSceneData(const QUrl& file_url) const noexcept
{
  QVariant scene_data;
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
                                       const QVariant& scene_data) const noexcept
{
  QString message;
  const bool result = SceneDocument::saveDocument(file_url, scene_data, message);
  if (!result) {
    qFatal("%s", message.toStdString().c_str());
  }
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
  */
void GuiRendererManager::connectWithRenderer(GuiRenderer* renderer) noexcept
{
  ZISC_ASSERT(renderer != nullptr, "The renderer is nulll.");
  {
    auto notify_of_info = [this](const QString& info)
    {
      emit notifyOfRenderingInfo(info);
    };
    connect(renderer, &GuiRenderer::notifiedOfRenderingInfo, notify_of_info);
  }
  {
    auto stop_rendering = [renderer]()
    {
      ZISC_ASSERT(renderer != nullptr, "The renderer is nulll.");
      renderer->setRunnable(false);
    };
    connect(this, &GuiRendererManager::stopRendering, stop_rendering);
  }
  {
    auto handle_camera_event = [renderer](const int transformation_event_type,
                                          const int axis_event_type,
                                          const int value)
    {
      ZISC_ASSERT(renderer != nullptr, "The renderer is nulll.");
      auto& camera_event = renderer->cameraEvent();
      camera_event.addEvent(transformation_event_type, axis_event_type, value);
    };
    connect(this, &GuiRendererManager::previewEvent, handle_camera_event);
  }
  auto image_provider = renderedImageProvider();
  image_provider->setImage(&renderer->ldrImageHelper());
}

/*!
  */
void GuiRendererManager::disconnectFromRenderer() noexcept
{
  disconnect(this, &GuiRendererManager::stopRendering, nullptr, nullptr);
  disconnect(this, &GuiRendererManager::previewEvent, nullptr, nullptr);
  auto image_provider = renderedImageProvider();
  image_provider->setImage(nullptr);
}

/*!
  */
RenderedImageProvider* GuiRendererManager::renderedImageProvider() noexcept
{
  ZISC_ASSERT(image_provider_ != nullptr, "The image provider is null.");
  return image_provider_;
}

} // namespace nanairo
