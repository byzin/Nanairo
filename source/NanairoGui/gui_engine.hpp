/*!
  \file gui_engine.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_GUI_ENGINE_HPP_
#define _NANAIRO_GUI_ENGINE_HPP_

// Qt
#include <QQmlApplicationEngine>
// Nanairo
#include "NanairoCore/Utility/scene_settings.hpp"
#include "Utility/gui_renderer_manager.hpp"

// Forward declaration
class QUrl;

namespace nanairo {

//! \addtogroup Gui
//! \{

// Forward declaration
class RenderedImageProvider;

/*!
  \details
  No detailed.
  */
class GuiEngine
{
 public:
  //! Initialize GUI engine
  GuiEngine();


  //! Load window
  void load(const QUrl& url);

 private:
  //! Initialize GUI engine
  void initialize();


  QQmlApplicationEngine engine_;
  GuiRendererManager manager_;
  SceneSettings settings_;
  RenderedImageProvider* image_provider_;
};

//! \} Gui

} // namespace nanairo

#endif // _NANAIRO_GUI_ENGINE_HPP_
