/*!
  \file cui_renderer_manager.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cui_renderer_manager.hpp"
// Standard C++ library
#include <string>
// Qt
#include <QDate>
#include <QDir>
#include <QFileInfo>
#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QString>
#include <QTime>
#include <QTextStream>
#include <QtGlobal>
// Zisc
#include "zisc/error.hpp"
#include "zisc/stopwatch.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"
#include "NanairoRenderer/cpu_scene_renderer.hpp"
#include "NanairoRenderer/renderer_utility.hpp"
#include "NanairoRenderer/scene_renderer_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
CuiRendererManager::CuiRendererManager(const QString& scene_file_path) noexcept :
    scene_file_path_{scene_file_path}
{
  initialize();
}

/*!
  \details
  No detailed.
  */
void CuiRendererManager::render() noexcept
{
  // Load the scene settings
  QScopedPointer<SceneSettings> settings{new SceneSettings};
  settings->open(scene_file_path_);

  // Make output directory
  const auto current_dir = QDir::current();
  const auto key = QString{keyword::system} + "/" + keyword::sceneName;
  const auto scene_name = settings->stringValue(key);
  const auto current_time = getCurrentTime();
  const auto dir_name = scene_name + "_" + current_time;
  current_dir.mkdir(dir_name);

  // Set the output path
  const auto current_path = current_dir.absolutePath();
  const auto output_dir = current_path + "/" + dir_name;

  // Make the renderer
  QScopedPointer<SceneRendererBase> renderer{new CpuSceneRenderer};
  setRenderer(renderer.data());

  // Render
  renderer->initialize(*settings);
  settings.reset(nullptr);
  renderer->renderImage(output_dir);
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

/*!
  \details
  No detailed.
  */
void CuiRendererManager::initialize() noexcept
{
  QFileInfo scene_file_info{scene_file_path_};
  if (!scene_file_info.exists())
    zisc::raiseError("scene file: \"", scene_file_path_.toStdString(), "\" not found.");
}

/*!
  \details
  No detailed.
  */
void CuiRendererManager::setRenderer(const SceneRendererBase* renderer) noexcept
{
  QSharedPointer<QTextStream> cout{new QTextStream{stdout}};

  auto set_rendering_info = [cout](const quint64 cycle, const qint64 time)
  {
    using zisc::cast;
    constexpr quint64 k = cast<quint64>(zisc::Stopwatch::Clock::period::den);
    const double fps = cast<double>(k * cycle) / cast<double>(time);
    const auto fps_string = QString::number(fps, 'f', 2);
    const auto cycle_string = QStringLiteral("%1").arg(cycle, 8, 10, QChar('0'));
    const auto time_string = getTimeString(zisc::Stopwatch::Clock::duration{time});

    *cout << "FPS: " << fps_string
          << " cycle: " << cycle_string 
          << " time: " << time_string << "\r";
    flush(*cout);
  };
  QObject::connect(renderer, &SceneRendererBase::updated, set_rendering_info);

  auto set_message = [cout](const QString& message)
  {
    *cout << message;
    endl(*cout);
  };
  QObject::connect(renderer, &SceneRendererBase::outputMessage, set_message);
}

} // namespace nanairo
