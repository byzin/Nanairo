/*!
  \file gui_renderer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_GUI_RENDERER_HPP
#define NANAIRO_GUI_RENDERER_HPP

// Standard C++ library
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
// Qt
#include <QObject>
// Zisc
#include "zisc/stopwatch.hpp"
// Nanairo
#include "camera_event.hpp"
#include "cui_renderer.hpp"
#include "simple_renderer.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declaration
class QString;

namespace nanairo {

//! \addtogroup Gui
//! \{

/*!
  */
class GuiRenderer : public QObject, public CuiRenderer
{
  Q_OBJECT

 public:
  using Clock = SimpleRenderer::Clock;
  enum class RenderingMode : int
  {
    kRendering,
    kPreviewing
  };


  //! Create a renderer
  GuiRenderer(const RenderingMode mode) noexcept;


  //! Return the camera event of the renderer
  CameraEvent& cameraEvent() noexcept;

  //! Return the camera event of the renderer
  const CameraEvent& cameraEvent() const noexcept;

 signals:
  // Notify of updating rendering infomation
  void notifiedOfRenderingInfo(const QString& info) const;

 private:
  //! Handle camera event
  void handleCameraEvent(zisc::Stopwatch* stopwatch,
                         uint32* cycle,
                         Clock::duration* time) noexcept override;

  //! Initialize the renderer
  void initialize() noexcept;

  //! Initialize logger
  void initLogger(const std::string& output_path,
                  std::ostream* console_log_stream,
                  std::ofstream* text_log_stream) noexcept override;

  //! Notify of updating rendering information
  void notifyOfRenderingInfo(const std::string_view& info) const noexcept override;

  //! Output LDR image
  void outputLdrImage(const std::string_view output_path,
                      const uint32 cycle,
                      const std::string_view suffix = "") noexcept override;


  CameraEvent camera_event_;
  RenderingMode mode_;
};

//! \} Gui

} // namespace nanairo

#include "gui_renderer-inl.hpp"

#endif // NANAIRO_GUI_RENDERER_HPP
