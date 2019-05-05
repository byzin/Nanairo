/*!
  \file cui_renderer_manager.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cui_renderer_manager.hpp"
// Standard C++ library
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
// Qt
#include <QDate>
#include <QDir>
#include <QImage>
#include <QJsonObject>
#include <QString>
#include <QTime>
#include <QTextStream>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "cui_renderer.hpp"
#include "scene_document.hpp"
#include "scene_value.hpp"
#include "simple_progress_bar.hpp"
#include "simple_renderer.hpp"
#include "NanairoCore/Setting/scene_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoGui/keyword.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
CuiRendererManager::CuiRendererManager() noexcept
    : is_saving_scene_binary_enabled_{false}
{
  initialize();
}

/*!
  */
void CuiRendererManager::enableSavingSceneBinary(const bool is_enabled) noexcept
{
  is_saving_scene_binary_enabled_ = is_enabled;
}

/*!
  */
void CuiRendererManager::invokeRendering(const QString& scene_file_path)
    const noexcept
{
  CuiRenderer renderer;
  std::unique_ptr<std::ofstream> log_stream;
  QImage image;
  QString output_dir;
  QString error_message;

  // Prepare for rendering
  {
    QJsonObject scene_value;
    if (SceneDocument::loadDocument(scene_file_path, scene_value, error_message)) {
      // Make a setting file
      const auto scene_settings = SceneValue::toSetting(scene_value);

      // Make a output directory
      output_dir = makeOutputDir(*scene_settings);
      if (!output_dir.isEmpty()) {
        backupSceneFiles(scene_value, *scene_settings, output_dir, &error_message);

        // Init log streams
        log_stream = makeTextLogStream(output_dir.toStdString());
        renderer.setLogStream(log_stream.get());

        // Init a scene
        prepareForRendering(*scene_settings, &renderer, &error_message);
      }
      else {
        error_message = "making output dir failed.";
      }
    }
  }

  if (!error_message.isEmpty())
    QTextStream{stderr} << "Error: " << error_message;

  // Make a progress bar
  SimpleProgressBar progress_bar;
  auto notify_of_progress =
  [&progress_bar](const double progress, const std::string_view status)
  {
    progress_bar.update(progress, status);
  };
  renderer.setProgressCallback(notify_of_progress);

  // Start rendering
  if (renderer.isRunnable()) {
    {
      const auto& ldr_image = renderer.ldrImage();
      image = QImage{static_cast<int>(ldr_image.widthResolution()),
                     static_cast<int>(ldr_image.heightResolution()),
                     QImage::Format_RGB32};
      renderer.setImage(&image);
    }
    renderer.render(output_dir.toStdString());
  }
}

/*!
  */
bool CuiRendererManager::isSavingSceneBinaryEnabled() const noexcept
{
  return is_saving_scene_binary_enabled_;
}

/*!
  */
const QString& CuiRendererManager::outputPath() const noexcept
{
  return output_path_;
}

/*!
  */
void CuiRendererManager::setOutputPath(const QString& output_path) noexcept
{
  output_path_ = output_path;
}

/*!
  */
void CuiRendererManager::backupSceneFiles(const QJsonObject& scene_value,
                                          const SettingNodeBase& scene_settings,
                                          const QString& output_dir,
                                          QString* error_message) const noexcept
{
  QString dummy_message;
  QString* err_message = (error_message != nullptr) ? error_message : &dummy_message;

  // Backup scene settings
  {
    const auto backup_path = output_dir + "/" + keyword::sceneBackupFileName;
    if (!SceneDocument::saveDocument(backup_path, scene_value, *err_message))
      return;
  }

  // Backup scene binary
  if (isSavingSceneBinaryEnabled()) {
    const auto backup_path = output_dir + "/" + keyword::sceneBinaryFileName;
    std::ofstream scene_binary{backup_path.toStdString(), std::ios::binary};
    if (!scene_binary.is_open()) {
      *err_message = "making scene binary failed.";
      return;
    }
    scene_settings.writeData(&scene_binary);
  }
}

/*!
  */
QString CuiRendererManager::makeOutputDir(const SettingNodeBase& scene_settings)
    const noexcept
{
  const auto settings = castNode<SceneSettingNode>(&scene_settings);
  const auto scene_name = QString{settings->sceneName().data()};

  QString dir_name = outputPath() + "/" + scene_name + "_" + getCurrentTime();
  const bool result = QDir::current().mkpath(dir_name);
  if (!result)
    dir_name.clear();
  return dir_name;
}

/*!
  */
bool CuiRendererManager::prepareForRendering(const SettingNodeBase& scene_settings,
                                             CuiRenderer* renderer,
                                             QString* error_message) const noexcept
{
  ZISC_ASSERT(renderer != nullptr, "The renderer is null.");

  {
    std::string message;
    const bool result = renderer->loadScene(scene_settings, &message);
    if ((error_message != nullptr) && !result)
      *error_message = message.c_str();
  }

  return renderer->isRunnable();
}

/*!
  \details
  No detailed.
  */
QString CuiRendererManager::getCurrentTime() const noexcept
{
  const auto current_date = QDate::currentDate().toString("yyyy_MM_dd");
  const auto current_time = QTime::currentTime().toString("HH_mm_ss");
  return current_date + "_" + current_time;
}

void CuiRendererManager::initialize() noexcept
{
}

} // namespace nanairo
