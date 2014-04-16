/*!
  \file scene_renderer_base.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SCENE_RENDERER_BASE_HPP_
#define _NANAIRO_SCENE_RENDERER_BASE_HPP_

// Qt
#include <QImage>
#include <QObject>
#include <QString>
#include <QtGlobal>
// Zisc
#include "zisc/stopwatch.hpp"
// Nanairo
#include "camera_event.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"

// Foward declaration
class QMatrix4x4;

namespace nanairo {

// Forward declaration
class SceneSettings;

//! \addtogroup Renderer
//! \{

/*!
  \details
  No detailed.
  */
class SceneRendererBase : public QObject
{
  Q_OBJECT

 public:
  //! Create a renderer
  SceneRendererBase();

  //! Finalize the renderer
  virtual ~SceneRendererBase();


  //! Initialize the renderer
  void initialize(const SceneSettings& settings);

  //! Return the LDR image format
  const QString& ldrImageFormat() const;

  //! Preview the rendered image
  void previewImage();

  //! Render the scene image
  void renderImage(const QString& output_dir);

  //! Return the rendered image
  const QImage& renderedImage() const;

 public slots:
  //! Handle mouse event in preview rendering
  void handlePreviewMouseEvent(const int buttons,
                               const int delta_x,
                               const int delta_y,
                               const int delta_wheel);

  //! Stop rendering
  void stopRendering() const;

 signals:
  //! Called when rendering is finished
  void finished() const;

  //! Output the transformation matrix
  void outputMatrix(const QMatrix4x4& matrix) const;

  //! Output message
  void outputMessage(const QString& message) const;

  //! Called when
  void stop() const;

  //! Called when rendered image updated.
  void updated(quint64 cycle, qint64 time) const;

 protected:
  using Clock = zisc::Stopwatch::Clock;


  //! Return the camera event
  const CameraEvent& cameraEvent() const;

  //! Return the camera matrix
  const Matrix4x4& cameraMatrix() const;

  //! Return the camera matrix
  Matrix4x4& cameraMatrix();

  //! Convert the spectra buffer to HDR XYZ buffer
  virtual void convertSpectraToHdr(const quint64 cycle) = 0;

  //! Handle the camera event
  virtual void handleCameraEvent() = 0;

  //! Initialize the renderer
  virtual void initializeRenderer(const SceneSettings& settings) = 0;

  //! Render the scene image
  virtual void render(const quint64 cycle) = 0;

  //! Return the rendered image
  QImage& renderedImage();

  //! Tonemap
  virtual void toneMap() = 0;

 private:
  //! Handle the camera event
  void handleCameraEvent(quint64* cycle, zisc::Stopwatch* stopwatch);

  //! Check if the current cycle is to save image
  bool isCycleToSaveImage(const quint64 cycle) const;

  //! Check if the current cycle is last
  bool isLastCycle(const quint64 cycle) const;

  //! Check if the current time is to save image
  bool isTimeToSaveImage(const Clock::duration& time, 
                         Clock::rep* interval_count) const;

  //! Output the transformation matrix
  void outputMatrix(const Matrix4x4& matrix) const;

  //! Save LDR iamge
  void saveLdrImage(const quint64 cycle, const QString& output_dir) const;

  //! Wait for next frame, if necessary
  bool waitForNextFrame(const Clock::duration& delta_time) const;


  Matrix4x4 camera_matrix_;
  QImage ldr_image_;
  QString ldr_image_format_;
  Clock::duration saving_interval_time_;
  CameraEvent camera_event_;
  quint64 termination_pass_;
  bool is_power2_saving_;
};

//! \} Renderer

} // namespace nanairo

#include "scene_renderer_base-inl.hpp"

#endif // _NANAIRO_SCENE_RENDERER_BASE_HPP_
