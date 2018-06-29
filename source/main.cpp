/*!
  \file main.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <cstdlib>
#include <functional>
#include <memory>
// Qt
#ifdef QT_WIDGETS_LIB
#include <QApplication>
#else // QT_WIDGETS_LIB
#include <QGuiApplication>
#endif // QT_WIDGETS_LIB
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QFont>
#include <QGuiApplication>
#include <QScopedPointer>
#include <QString>
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
#include "NanairoGui/Renderer/scene_document.hpp"

namespace {

/*!
  */
enum class RendererManagerType
{
  kCui,
  kGui
};

/*!
  */
struct NanairoParameters
{
  QString scene_file_path_;
  QString output_path_;
  RendererManagerType manager_type_ = RendererManagerType::kGui;
  bool is_saving_scene_binary_enabled_ = false;
};

//! Initialize Qt settings
void initQt() noexcept;

//! Process command-line parameters
std::unique_ptr<NanairoParameters> processCommandLine(
    const QCoreApplication& application) noexcept;

//! Make a renderer runner
std::function<int ()> makeRenderRunner(
    const QCoreApplication& application,
    const NanairoParameters& parameters) noexcept;

} // namespace

/*!
  \details
  No detailed.
  */
int main(int argc, char** argv)
{
  // Initialize application
  QScopedPointer<QGuiApplication> application{
#ifdef QT_WIDGETS_LIB
    new QApplication{argc, argv}
#else // QT_WIDGETS_LIB
    new QGuiApplication{argc, argv}
#endif // QT_WIDGETS_LIB
    };
  initQt();

  // Parse command line arguments
  const auto parameters = processCommandLine(*application);
  // Make a render runner
  const auto render_runner = makeRenderRunner(*application, *parameters);
  // Launch render runner
  return render_runner();
}

namespace {

/*!
  */
void initQt() noexcept
{
  // Application info
  QGuiApplication::setApplicationName(
      nanairo::GuiConfig::applicationName().c_str());
  QGuiApplication::setApplicationVersion(
      nanairo::CoreConfig::versionString().c_str());

  // Qt properties
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  // Random seed
  {
    const qint64 time = QDateTime::currentMSecsSinceEpoch();
    const qint32* array = zisc::treatAs<const qint32*>(&time);
    const qint32 seed = std::abs(array[0] ^ array[1]);
    qsrand(zisc::cast<uint>(seed));
  }
}

/*!
  \details
  No detailed.
  */
std::unique_ptr<NanairoParameters> processCommandLine(
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
      {"r", "renderermode"},
      "Choose gui/cui renderer to run.",
      "mode",
      "gui"};
  parser.addOption(renderer_mode_option);
  // Working directory
  QCommandLineOption working_directory_option{
      {"w", "workingdirectory"},
      "Set the current working directory of Nanairo application.",
      "path"};
  parser.addOption(working_directory_option);
  // Scene file
  QCommandLineOption scene_file_path_option{
      {"s", "scenefile"},
      "Specify the rendering scene for cui rendering.",
      "path",
      ":/NanairoGui/scene/DefaultScene.nana"};
  parser.addOption(scene_file_path_option);
  // Output path
  QCommandLineOption output_path_option{
      {"o", "outputfilepath"},
      "Specify the output path of rendered images.",
      "path",
      "results"};
  parser.addOption(output_path_option);
  // Scene binary
  QCommandLineOption scene_binary_option{
      {"b", "savescenebinary"},
      "Enable saving scene binary file when rendering."};
  parser.addOption(scene_binary_option);

  // Process parameters
  parser.process(application);

  // Set parameters
  auto parameters = std::make_unique<NanairoParameters>();
  // Rendering mode
  {
    const auto mode = parser.value(renderer_mode_option);
    if (mode == "gui")
      parameters->manager_type_ = RendererManagerType::kGui;
    else if (mode == "cui")
      parameters->manager_type_ = RendererManagerType::kCui;
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
  // Scene file path
  {
    const auto file_path = parser.value(scene_file_path_option);
    QString error_message;
    if (!nanairo::SceneDocument::isSceneDocument(file_path, error_message))
      zisc::raiseError(error_message.toStdString());
    parameters->scene_file_path_ = file_path;
  }
  // Output path
  {
    const auto output_path = parser.value(output_path_option);
    QDir::current().mkpath(output_path);
    const QFileInfo output_dir{output_path};
    if (!output_dir.isWritable())
      zisc::raiseError("The output dir isn't writable: ", output_path.toStdString());
    parameters->output_path_ = output_dir.canonicalFilePath();
  }
  // Scene binary
  if (parser.isSet(scene_binary_option)) {
    parameters->is_saving_scene_binary_enabled_ = true;
  }

  return parameters;
}

/*!
  */
std::function<int ()> makeRenderRunner(
    const QCoreApplication& application,
    const NanairoParameters& parameters) noexcept
{
  std::function<int ()> render_runner;
  switch (parameters.manager_type_) {
    case RendererManagerType::kGui: {
      render_runner = [&application, &parameters]()
      {
        // Initialize GUI
        nanairo::GuiEngine engine;
        engine.load(QUrl{"qrc:/NanairoGui/NMainWindow.qml"});
        auto& manager = engine.rendererManager();
        manager.enableSavingSceneBinary(parameters.is_saving_scene_binary_enabled_);
        manager.setOutputPath(parameters.output_path_);
        return application.exec();
      };
      break;
    }
    case RendererManagerType::kCui: {
      render_runner = [&parameters]()
      {
        nanairo::CuiRendererManager manager;
        manager.enableSavingSceneBinary(parameters.is_saving_scene_binary_enabled_);
        manager.setOutputPath(parameters.output_path_);
        manager.invokeRendering(parameters.scene_file_path_);
        return 0;
      };
      break;
    }
    default:
      break;
  }
  return render_runner;
}

} // namespace
