/*!
  \file main.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <cstdlib>
#include <functional>
#include <memory>
// Qt
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFont>
#include <QGuiApplication>
#include <QScopedPointer>
#include <QString>
#include <QStringList>
#include <QtGlobal>
#include <QQmlApplicationEngine>
#include <QUrl>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoGui/gui_engine.hpp"
#include "NanairoGui/nanairo_gui_config.hpp"
#include "NanairoGui/Renderer/cui_renderer_manager.hpp"
#include "NanairoGui/Renderer/renderer_parameter.hpp"
#include "NanairoGui/Renderer/scene_document.hpp"

namespace {

//! Make a renderer runner
std::function<int ()> makeRendererRunner(
    const QCoreApplication& application,
    const std::shared_ptr<nanairo::RendererParameter>& options) noexcept;

//! Process command-line options
std::shared_ptr<nanairo::RendererParameter> processCommandLineOptions(
    const QCoreApplication& application) noexcept;

//! Initialize Qt random seed
void initializeQtRandomSeed() noexcept;

} // namespace

/*!
  \details
  No detailed.
  */
int main(int argc, char** argv)
{
  using nanairo::CoreConfig;
  using nanairo::GuiConfig;

  // Initialize application
  QScopedPointer<QGuiApplication> application{new QGuiApplication{argc, argv}};
  QCoreApplication::setApplicationName(GuiConfig::applicationName().c_str());
  QCoreApplication::setApplicationVersion(CoreConfig::versionString().c_str());
  const auto options = processCommandLineOptions(*application);

  // Initialize Qt
  initializeQtRandomSeed();

  auto renderer_runner = makeRendererRunner(*application, options);
  return renderer_runner();
}

namespace {

/*!
  */
std::function<int ()> makeRendererRunner(
    const QCoreApplication& application,
    const std::shared_ptr<nanairo::RendererParameter>& options) noexcept
{
  std::function<int ()> renderer_runner;
  switch (options->managerType()) {
    case nanairo::RendererManagerType::kGui: {
      renderer_runner = [&application]()
      {
        // Initialize GUI
        nanairo::GuiEngine engine;
        engine.load(QUrl{"qrc:/NanairoGui/NMainWindow.qml"});
        return application.exec();
      };
      break;
    }
    case nanairo::RendererManagerType::kCui: {
      renderer_runner = [options]()
      {
        nanairo::CuiRendererManager manager{*options};
        const bool is_runnable = manager.isRunnable();
        if (is_runnable)
          manager.invokeRenderer();
        return is_runnable ? 0 : EXIT_FAILURE;
      };
      break;
    }
    default:
      break;
  }
  return renderer_runner;
}

/*!
  \details
  No detailed.
  */
std::shared_ptr<nanairo::RendererParameter> processCommandLineOptions(
    const QCoreApplication& application) noexcept
{
  QCommandLineParser parser;

  // Application description
  const auto application_description = QStringLiteral(
      "Nanairo is a physically plausible spectral renderer.");
  parser.setApplicationDescription(application_description);

  // Help and version setting
  parser.addHelpOption();
  parser.addVersionOption();

  // Options
  // Rendering mode
  QCommandLineOption renderer_mode_option{
      QStringList{} << "r" << "renderer-mode",
      "Choose gui/cui renderer to run.",
      "mode",
      "gui"};
  parser.addOption(renderer_mode_option);
  // Working directory
  QCommandLineOption working_directory_option{
      QStringList{} << "w" << "working-directory",
      "Set the Nanairo's current working directory.",
      "dir"};
  parser.addOption(working_directory_option);
  // Scene file
  QCommandLineOption scene_file_option{
    QStringList{} << "s" << "scene-file",
    "Specify the rendering scene.",
    "path",
    ":/NanairoGui/scene/DefaultScene.nana"};
  parser.addOption(scene_file_option);

  // Process options
  parser.process(application);

  // Set options
  auto options = std::make_shared<nanairo::RendererParameter>();
  // Rendering mode
  {
    const auto mode = parser.value(renderer_mode_option);
    if (mode == "gui")
      options->setManagerType(nanairo::RendererManagerType::kGui);
    else if (mode == "cui")
      options->setManagerType(nanairo::RendererManagerType::kCui);
    else
      zisc::raiseError("Invalid renderer mode is specified: ", mode.toStdString());
  }
  // Working directory
  if (parser.isSet(working_directory_option)) {
    const auto dir = parser.value(working_directory_option);
    const bool result = QDir::setCurrent(dir);
    if (!result)
      zisc::raiseError("Invalid working directory is specified: ", dir.toStdString());
  }
  // Scene file
  {
    const auto path = parser.value(scene_file_option);
    QString error_message;
    if (!nanairo::SceneDocument::checkDocumentInfo(path, error_message))
      zisc::raiseError(error_message.toStdString());
    options->setSceneFilePath(path);
  }

  return options;
}

/*!
  \details
  No detailed.
  */
void initializeQtRandomSeed() noexcept
{
  using zisc::cast;

  const qint64 time = QDateTime::currentMSecsSinceEpoch();
  const qint32* array = zisc::treatAs<const qint32*>(&time);
  const qint32 seed = std::abs(array[0] ^ array[1]);
  qsrand(cast<uint>(seed));
}

} // namespace
