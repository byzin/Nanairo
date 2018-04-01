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
#include <string>
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
                         uint64* cycle,
                         Clock::duration* time) noexcept override;

  //! Initialize the renderer
  void initialize() noexcept;

  //! Log a message
  void logMessage(const std::string& message) noexcept override;

  //! Notify of updating rendering information
  void notifyOfRenderingInfo(const std::string& info) const noexcept override;

  //! Output LDR image
  void outputLdrImage(const std::string& output_path,
                      const uint64 cycle) noexcept override;


  CameraEvent camera_event_;
  RenderingMode mode_;
};

//! \} Gui

} // namespace nanairo

#include "gui_renderer-inl.hpp"

#endif // NANAIRO_GUI_RENDERER_HPP
