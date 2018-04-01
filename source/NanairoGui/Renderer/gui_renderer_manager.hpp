/*!
  \file gui_renderer_manager.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_GUI_RENDERER_MANAGER
#define NANAIRO_GUI_RENDERER_MANAGER

// Standard C++ library
#include <cstdint>
// Qt
#include <QObject>
#include <QString>
#include <QUrl>
#include <QVariant>
#include <QtGlobal>
// Zisc
#include "zisc/thread_pool.hpp"
// Nanairo
#include "cui_renderer_manager.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Gui
//! \{

// Forward declaration
class GuiRenderer;
class RenderedImageProvider;
class SceneRendererBase;

/*!
  \details
  No detailed.
  */
class GuiRendererManager : public QObject, public CuiRendererManager
{
  Q_OBJECT

 public:
  //! Initialize manager
  GuiRendererManager() noexcept;


  // Utilities
  //! Return the current working directory
  Q_INVOKABLE QString currentWorkingDir() const noexcept;

  //! Generate a random seed randomly
  Q_INVOKABLE int generateSeedRandomly() const noexcept;

  //! Get the file name from URL
  Q_INVOKABLE QString getFileName(const QUrl& file_path) const noexcept;

  //! Return the ideal thread counts
  Q_INVOKABLE int getIdealThreadCount() const noexcept;

  //! Convert a relative path to a url of absolute path
  Q_INVOKABLE QUrl getAbsolutePath(const QString& relative_path) const noexcept;

  //! Convert a url of absolute path to a relative path
  Q_INVOKABLE QString getRelativePath(const QUrl& absolute_path) const noexcept;

  //! Check if the GUI is debug mode
  Q_INVOKABLE bool isDebugMode() const noexcept;

  //! Invoke the rendering function of the renderer
  Q_INVOKABLE void invokeRendering(const QVariant& scene_data,
                                   const bool is_previewing) noexcept;

  //! Open a scene data
  Q_INVOKABLE QVariant loadSceneData(const QUrl& file_path) const noexcept;

  //! Save a scene data
  Q_INVOKABLE void saveSceneData(const QUrl& file_path,
                                 const QVariant& scene_data) const noexcept;

  //! Set rendered image provider
  void setRenderedImageProvider(RenderedImageProvider* image_provider) noexcept;

 signals:
  //! Notify that rendering is finished
  void finished() const;

  //! Notify that rendering is started
  void started() const;

  //! Called when the manager is to stop the rendering
  void stopRendering() const;

  //! Called when a preview event is occured
  void previewEvent(const int transformation_event_type,
                    const int axis_event_type,
                    const int value) const;

  //! Notify of updating rendering info
  void notifyOfRenderingInfo(const QString& info) const;

 private:
  //! Connect with renderer
  void connectWithRenderer(GuiRenderer* renderer) noexcept;

  //! Disconnect from renderer
  void disconnectFromRenderer() noexcept;

  //! Return the image provider
  RenderedImageProvider* renderedImageProvider() noexcept;


  zisc::ThreadPool rendering_thread_;
  RenderedImageProvider* image_provider_;
};

//! \} Gui

} // namespace nanairo

#endif // NANAIRO_GUI_RENDERER_MANAGER
