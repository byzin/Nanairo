/*!
  \file gui_renderer_manager.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_renderer_manager.hpp"
// Standard C++ library
#include <cstdint>
#include <fstream>
#include <memory>
#include <utility>
// Qt
#include <QColor>
#include <QFileInfo>
#include <QFont>
#include <QFontDatabase>
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
#include "scene_value.hpp"
#include "simple_renderer.hpp"
#include "NanairoGui/keyword.hpp"
#include "NanairoGui/nanairo_gui_config.hpp"
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
QString GuiRendererManager::getDefaultFixedFontFamily() const noexcept
{
  const auto name = GuiConfig::getDefaultFixedFontFamily();
  return QString{name.data()};
}

/*!
  */
QString GuiRendererManager::getDefaultFontFamily() const noexcept
{
  const auto name = GuiConfig::getDefaultFontFamily();
  return QString{name.data()};
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
  auto render = [this, scene_value, is_previewing]() mutable
  {
    const GuiRenderer::RenderingMode mode = (is_previewing)
        ? GuiRenderer::RenderingMode::kPreviewing
        : GuiRenderer::RenderingMode::kRendering;
    QScopedPointer<GuiRenderer> renderer{new GuiRenderer{mode}};
    std::unique_ptr<std::ofstream> log_stream;
    QString output_dir;
    QString error_message;

    // Prepare for rendering
    {
      // Make a setting file
      const auto scene_settings = SceneValue::toSetting(*scene_value);

      // Make a output directory
      output_dir = makeOutputDir(*scene_settings);
      if (!output_dir.isEmpty()) {
        backupSceneFiles(*scene_value, *scene_settings, output_dir, &error_message);

        // Init log stream
        log_stream = makeTextLogStream(output_dir.toStdString());
        renderer->setLogStream(log_stream.get());

        // Init a scene
        emit started();
        prepareForRendering(*scene_settings, renderer.get(), &error_message);
      }
      else {
        error_message = "making output dir failed.";
      }
    }
    scene_value.reset();

    if (!error_message.isEmpty())
      QTextStream{stderr} << "Error: " << error_message;

    // Start rendering 
    if (renderer->isRunnable()) {
      // Init image
      {
        const auto& ldr_image = renderer->ldrImage();
        auto image_provider = renderedImageProvider();
        auto& image = image_provider->image();
        image = QImage{static_cast<int>(ldr_image.widthResolution()),
                       static_cast<int>(ldr_image.heightResolution()),
                       QImage::Format_RGB32};
        image.fill(QColor::fromRgb(0, 0, 0));
        renderer->setImage(&image);
      }

      // Connect a renderer with this manager
      auto notify_of_progress =
      [this](const double progress, const std::string_view status)
      {
        emit notifyOfRenderingProgress(progress, QString{status.data()});
      };
      renderer->setProgressCallback(notify_of_progress);
      connectWithRenderer(renderer.get());

      // Start rendering
      renderer->render(output_dir.toStdString());

      // Finish rendering
      disconnectFromRenderer();
      emit finished();
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
}

/*!
  */
void GuiRendererManager::connectWithRenderer(GuiRenderer* renderer) noexcept
{
  ZISC_ASSERT(renderer != nullptr, "The renderer is nulll.");
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
}

/*!
  */
void GuiRendererManager::disconnectFromRenderer() noexcept
{
  disconnect(this, &GuiRendererManager::stopRendering, nullptr, nullptr);
  disconnect(this, &GuiRendererManager::previewEvent, nullptr, nullptr);
}

/*!
  */
RenderedImageProvider* GuiRendererManager::renderedImageProvider() noexcept
{
  ZISC_ASSERT(image_provider_ != nullptr, "The image provider is null.");
  return image_provider_;
}

} // namespace nanairo
