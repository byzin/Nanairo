/*!
  \file cui_renderer_manager.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cui_renderer_manager.hpp"
// Standard C++ library
#include <fstream>
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
  QImage image;
  QString output_dir;
  QString error_message;

  // Prepare for rendering
  {
    QJsonObject scene_value;
    if (SceneDocument::loadDocument(scene_file_path, scene_value, error_message))
      prepareForRendering(scene_value, &renderer, &output_dir, &error_message);
  }

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
  else {
    QTextStream{stderr} << "Error: " << error_message;
  }
}

bool CuiRendererManager::isSavingSceneBinaryEnabled() const noexcept
{
  return is_saving_scene_binary_enabled_;
}

/*!
  */
bool CuiRendererManager::prepareForRendering(const QJsonObject& scene_value,
                                             CuiRenderer* renderer,
                                             QString* output_dir,
                                             QString* error_message) const noexcept
{
  ZISC_ASSERT(renderer != nullptr, "The renderer is null.");
  ZISC_ASSERT(output_dir != nullptr, "The output dir var is null.");
  ZISC_ASSERT(error_message != nullptr, "The error message var is null.");

  // Make a setting file
  const auto scene_settings = SceneValue::toSetting(scene_value);

  // Make a output directory
  *output_dir = makeOutputDir(scene_settings.get());
  if (output_dir->isEmpty()) {
    *error_message = "Error: making output dir failed.";
    return false;
  }

  // Backup scene settings
  {
    const auto backup_path = (*output_dir) + "/" + keyword::sceneBackupFileName;
    if (!SceneDocument::saveDocument(backup_path, scene_value, *error_message))
      return false;
  }

  // Backup scene binary
  if (isSavingSceneBinaryEnabled()) {
    const auto backup_path = (*output_dir) + "/" + keyword::sceneBinaryFileName;
    std::ofstream scene_binary{backup_path.toStdString(), std::ios::binary};
    if (!scene_binary.is_open()) {
      *error_message = "Error: making scene binary file failed.";
      return false;
    }
    scene_settings->writeData(&scene_binary);
  }

  {
    std::string message;
    const bool result = renderer->loadScene(*scene_settings, &message);
    if (!result)
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

/*!
  */
QString CuiRendererManager::makeOutputDir(const SettingNodeBase* settings)
    const noexcept
{
  const auto scene_settings = castNode<SceneSettingNode>(settings);
  const auto scene_name = QString{scene_settings->sceneName().data()};

  QString dir_name = "results/" + scene_name + "_" + getCurrentTime();
  const bool result = QDir::current().mkpath(dir_name);
  if (!result)
    dir_name.clear();
  return dir_name;
}

} // namespace nanairo
