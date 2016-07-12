/*!
  \file main.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <cstdlib>
#include <functional>
// Qt
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCoreApplication>
#include <QDateTime>
#include <QFont>
#include <QGuiApplication>
#include <QString>
#include <QStringList>
#include <QtGlobal>
#include <QQmlApplicationEngine>
#include <QUrl>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCommon/nanairo_common_config.hpp"
#include "NanairoGui/gui_engine.hpp"
#include "NanairoRenderer/cui_renderer_manager.hpp"

/*!
  \details
  No detailed.
  */
enum class RendererMode
{
  Cui,
  Gui
};

/*!
  \details
  No detailed.
  */
struct Options
{
  RendererMode mode_ = RendererMode::Gui;
  QString scene_file_path_;
};

//! Get the renderer runner
std::function<int ()> getRendererRunner(QCoreApplication& application,
                                        const Options& options);

//! Process command-line options
Options processCommandLineOptions(const QCoreApplication& application);

//! Initialize Qt random seed
void initializeQtRandomSeed();

/*!
  \details
  No detailed.
  */
int main(int argc, char** argv)
{
  // Initialize application
  QGuiApplication application{argc, argv};
  QCoreApplication::setApplicationName(nanairo::kApplicationName);
  QCoreApplication::setApplicationVersion(nanairo::kVersionString);
  const auto options = processCommandLineOptions(application);

  // Initialize Qt
  initializeQtRandomSeed();
  // Font
#ifdef Z_LINUX
  auto font = application.font();
  font.setPointSize(8);
  application.setFont(font);
#endif // Z_LINUX

  auto run_renderer = getRendererRunner(application, options);
  return run_renderer();
}

/*!
  \details
  No detailed.
  */
std::function<int ()> getRendererRunner(QCoreApplication& application,
                                        const Options& options)
{
  std::function<int ()> run_renderer{[](){return 0;}};
  switch (options.mode_) {
   case RendererMode::Gui:
    run_renderer = [&application]()
    {
      // Initialize GUI
      nanairo::GuiEngine engine;
      engine.load(QUrl{"qrc:/NanairoGui/NMainWindow.qml"});
      return application.exec();
    };
    break;
   case RendererMode::Cui:
    run_renderer = [&options]()
    {
      nanairo::CuiRendererManager manager{options.scene_file_path_};
      manager.render();
      return 0;
    };
    break;
   default:
    break;
  }
  return run_renderer;
}

/*!
  \details
  No detailed.
  */
Options processCommandLineOptions(const QCoreApplication& application)
{
  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();

  const auto application_description = QStringLiteral(
      "Nanairo is a physically based spectral renderer.");
  parser.setApplicationDescription(application_description);

  // Options
  QCommandLineOption cui_rendering_option{
      QStringList{} << "cui" << "cui_rendering",
      "Run a console renderer instead of GUI.",
      "scene_file",
      "./resources/scene/DefaultScene.nana"};
  parser.addOption(cui_rendering_option);

  parser.process(application);

  // Set options
  Options options;
  if (parser.isSet(cui_rendering_option)) {
    options.mode_ = RendererMode::Cui;
    options.scene_file_path_ = parser.value(cui_rendering_option);
  }

  return options;
}

/*!
  \details
  No detailed.
  */
void initializeQtRandomSeed()
{
  using zisc::cast;

  const qint64 time = QDateTime::currentMSecsSinceEpoch();
  const qint32* array = zisc::treatAs<const qint32*>(&time);
  const qint32 seed = std::abs(array[0] | array[1]);
  qsrand(cast<uint>(seed));
}
