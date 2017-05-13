/*!
  \file gui_renderer_manager.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_GUI_RENDERER_MANAGER
#define NANAIRO_GUI_RENDERER_MANAGER

// Standard C++ library
#include <cstdint>
// Qt
#include <QMatrix4x4>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QVariant>
#include <QtGlobal>
// Zisc
#include "zisc/thread_pool.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Gui
//! \{

// Forward declaration
class RenderedImageProvider;
class SceneRendererBase;

/*!
  \details
  No detailed.
  */
class GuiRendererManager : public QObject
{
  Q_OBJECT

 public:
  //! Initialize manager
  GuiRendererManager() noexcept;


  // Utilities
  //! Return the current working directory
  Q_INVOKABLE QString currentWorkingDir() const noexcept;

  //! Get the file name from URL
  Q_INVOKABLE QString getFileName(const QUrl& file_path) const noexcept;

  //! Return the ideal thread counts
  Q_INVOKABLE int idealThreadCount() const noexcept;

  //! Check if the GUI is debug mode
  Q_INVOKABLE bool isDebugMode() const noexcept;

  //! Make the directory
  Q_INVOKABLE void makeDir(const QString& dir) const noexcept;

  //! Return the random number
  Q_INVOKABLE int random() const noexcept;

  //! Convert a relative path to a url of absolute path
  Q_INVOKABLE QUrl toAbsoluteFileUrl(const QString& relative_path) const noexcept;

  //! Convert a url of absolute path to a relative path
  Q_INVOKABLE QString toRelativeFilePath(const QUrl& absolute_path) const noexcept;

  // Manager
  //! Invoke the rendering function of thr renderer
  Q_INVOKABLE void invokeRendering(const QString& output_dir) noexcept;

  //! Invoke the previewing function of the renderer
  Q_INVOKABLE void invokePreviewing() noexcept;

  //! Open a scene data
  Q_INVOKABLE QVariantMap loadSceneData(const QUrl& file_path) const noexcept;

  //! Save a scene data
  Q_INVOKABLE void saveSceneData(const QUrl& file_path,
                                 const QVariantMap& scene_data) const noexcept;

  //! Set rendered image provider
  void setRenderedImageProvider(RenderedImageProvider* image_provider) noexcept;

 signals:
  //! Called when the rendering is finished
  void finished() const;

  //! Called when the rendering is started
  void started() const;

  //! Called when the manager is to stop the rendering
  void stopping() const;

  //! Called when rendered image is updated
  void updated(QString fps, QString cycle, QString time) const;

  //! Output the total camera event
  void cameraEventHandled(QMatrix4x4 matrix) const;

  //! Called when a preview event is occured
  void previewEvent(const int transformation_event_type,
                    const int axis_event_type,
                    const int value) const;

 private:
  //! Set renderer
  void setRenderer(const SceneRendererBase* renderer) noexcept;


  zisc::ThreadPool rendering_thread_;
  RenderedImageProvider* image_provider_;

 private slots:
  //! Finish rendering
  void finishRendering() noexcept;

  //! Set rendering information
  void setRenderingInfo(quint64 cycle, qint64 time) noexcept;

  //! Output the total camera event
  void outputCameraEvent(const QMatrix4x4& matrix) const noexcept;
};

//! \} Gui

} // namespace nanairo

#endif // NANAIRO_GUI_RENDERER_MANAGER
