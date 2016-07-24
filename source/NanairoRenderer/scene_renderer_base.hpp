/*!
  \file scene_renderer_base.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SCENE_RENDERER_BASE_HPP
#define NANAIRO_SCENE_RENDERER_BASE_HPP

// Qt
#include <QImage>
#include <QMatrix4x4>
#include <QObject>
#include <QString>
#include <QtGlobal>
// Zisc
#include "zisc/stopwatch.hpp"
// Nanairo
#include "camera_event.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"

// Foward declaration
class QJsonObject;

namespace nanairo {

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
  SceneRendererBase() noexcept;

  //! Finalize the renderer
  virtual ~SceneRendererBase() noexcept;


  //! Initialize the renderer
  void initialize(const QJsonObject& settings) noexcept;

  //! Preview the rendered image
  void previewImage() noexcept;

  //! Render the scene image
  void renderImage(const QString& output_dir) noexcept;

  //! Return the rendered image
  const QImage& renderedImage() const noexcept;

 public slots:
  //! Handle a preview event
  void handlePreviewEvent(const int transformation_event_type,
                          const int axis_event_type,
                          const int value) noexcept;

  //! Stop rendering
  void stopRendering() const noexcept;

 signals:
  //! Called when rendering is finished
  void finished() const;

  //! Output the total camera event 
  void cameraEventHandled(const QMatrix4x4& matrix) const;

  //! Output message
  void outputMessage(const QString& message) const;

  //! Called when
  void stop() const;

  //! Called when rendered image updated.
  void updated(quint64 cycle, qint64 time) const;

 protected:
  using Clock = zisc::Stopwatch::Clock;


  //! Return the camera event
  CameraEvent& cameraEvent() noexcept;

  //! Return the camera event
  const CameraEvent& cameraEvent() const noexcept;

  //! Return the camera transformation matrix
  Matrix4x4& cameraMatrix() noexcept;

  //! Return the camera transformation matrix
  const Matrix4x4& cameraMatrix() const noexcept;

  //! Convert the spectra buffer to HDR XYZ buffer
  virtual void convertSpectraToHdr(const quint64 cycle) noexcept = 0;

  //! Handle the camera event
  virtual void handleCameraEvent() noexcept = 0;

  //! Initialize the renderer
  virtual void initializeRenderer(const QJsonObject& settings) noexcept = 0;

  //! Render the scene image
  virtual void render(const quint64 cycle) noexcept = 0;

  //! Return the rendered image
  QImage& renderedImage() noexcept;

  //! Tonemap
  virtual void toneMap() noexcept = 0;

 private:
  //! Handle the camera event
  void handleCameraEvent(quint64* cycle, zisc::Stopwatch* stopwatch) noexcept;

  //! Check if the current cycle is to save image
  bool isCycleToSaveImage(const quint64 cycle) const noexcept;

  //! Check if the current cycle is last
  bool isLastCycle(const quint64 cycle) const noexcept;

  //! Check if the current time is to save image
  bool isTimeToSaveImage(const Clock::duration& time, 
                         Clock::rep* interval_count) const noexcept;

  //! Output the transformation matrix
  void outputCameraEvent() const noexcept;

  //! Save LDR iamge
  void saveLdrImage(const quint64 cycle, const QString& output_dir) const noexcept;

  //! Wait for next frame, if necessary
  bool waitForNextFrame(const Clock::duration& delta_time) const noexcept;


  Matrix4x4 camera_matrix_;
  QImage ldr_image_;
  Clock::duration saving_interval_time_;
  CameraEvent camera_event_;
  quint64 termination_pass_;
  bool is_power2_saving_;
};

//! \} Renderer

} // namespace nanairo

#include "scene_renderer_base-inl.hpp"

#endif // NANAIRO_SCENE_RENDERER_BASE_HPP
