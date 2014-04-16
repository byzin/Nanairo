/*!
  \file gui_renderer_manager.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_GUI_RENDERER_MANAGER_
#define _NANAIRO_GUI_RENDERER_MANAGER_

// Standard C++ library
#include <cstdint>
// Qt
#include <QMatrix4x4>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QtGlobal>
// Zisc
#include "zisc/thread_pool.hpp"

namespace nanairo {

//! \addtogroup Gui
//! \{

// Forward declaration
class SceneSettings;
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
  GuiRendererManager();


  //! Set rendered image provider
  void setRenderedImageProvider(RenderedImageProvider* image_provider);
    
  //! Set scene settings
  void setSceneSettings(const SceneSettings* settings);

  //! Return the current working directory
  Q_INVOKABLE QString currentWorkingDir() const;

  //! Return the default random seed
  Q_INVOKABLE int defaultRandomSeed() const;

  //! Get the file name from URL
  Q_INVOKABLE QString getFileName(const QUrl& file_path) const;

  //! Return the ideal thread counts
  Q_INVOKABLE int idealThreadCount() const;

  //! Make the directory
  Q_INVOKABLE void makeDir(const QString& dir) const;

  //! Render image
  Q_INVOKABLE void render(const QString& output_dir);

  //! Preview rendered image
  Q_INVOKABLE void preview();

  //! Return the random number
  Q_INVOKABLE int random() const;

 public slots:
  //! Set output message to Log view
  void setMessage(const QString& message) const;

 signals:
  //! Called when rendering is finished
  void finished() const;

  //! Called when rendering is started
  void started() const;

  //! Called when
  void stopRendering() const;

  //! Called when rendered image is updated
  void updated(QString fps, QString cycle, QString time);

  //! Output the camera matrix
  void outputCameraMatrix(const QMatrix4x4& matrix) const;

  //! Output a message
  void outputMessage(const QString& message) const;

  //! Called when mouse event is occured in preview rendering
  void previewMouseEvent(const int buttons, 
                         const int delta_x, 
                         const int delta_y, 
                         const int delta_wheel) const;

 private:
  //! Set renderer
  void setRenderer(const SceneRendererBase* renderer);


  zisc::ThreadPool rendering_thread_;
  const SceneSettings* settings_;
  RenderedImageProvider* image_provider_;

 private slots:
  //! Add the camera matrix 
  void addCameraMatrix(const QMatrix4x4& matrix);

  //! Finish rendering
  void finishRendering();

  //! Set rendering information
  void setRenderingInfo(quint64 cycle, qint64 time);
};

//! \} Gui

} // namespace nanairo

#include "gui_renderer_manager-inl.hpp"

#endif // _NANAIRO_GUI_RENDERER_MANAGER_
