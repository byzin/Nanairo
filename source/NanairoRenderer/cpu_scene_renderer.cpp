/*!
  \file cpu_scene_renderer.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cpu_scene_renderer.hpp"
// Standard C++ library
#include <cstddef>
#include <functional>
// Qt
#include <QLocale>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
// Zisc
#include "zisc/unit.hpp"
// Nanairo
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/hdr_image.hpp"
#include "NanairoCore/RenderingMethod/rendering_method.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/ToneMappingOperator/tone_mapping_operator.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
CpuSceneRenderer::CpuSceneRenderer() noexcept
{
}

/*!
  \details
  No detailed.
  */
void CpuSceneRenderer::convertSpectraToHdr(const quint64 cycle) noexcept
{
  auto& film = scene_->film();
  auto& spectra_image = film.spectraBuffer();
  spectra_image.toHdrImage(*system_, cycle, hdr_image_.get());
}

/*!
  \details
  No detailed.
  */
void CpuSceneRenderer::handleCameraEvent() noexcept
{
  auto& camera_event = cameraEvent();
  auto& camera = scene_->camera();
  auto& matrix = cameraMatrix();
  // Transform camera
  if (camera_event.isHorizontalTranslationEventOccured()) {
    const auto value = camera_event.flushHorizontalTranslationEvent();
    matrix = matrix * camera.translateHorizontally(value);
  }
  if (camera_event.isVerticalTranslationEventOccured()) {
    const auto value = camera_event.flushVerticalTranslationEvent();
    matrix = matrix * camera.translateVertically(value);
  }
  if (camera_event.isRotationEventOccured()) {
    const auto value = camera_event.flushRotationEvent();
    matrix = matrix * camera.rotate(value);
  }
  cameraMatrix() = matrix;
  
  // Initialize memory
  auto& film = scene_->film();
  film.spectraBuffer().clear();
  rgb_rendering_method_->clear();
  rendering_method_->clear();
}

/*!
  \details
  No detailed.
  */
void CpuSceneRenderer::initializeRenderer(const QJsonObject& settings) noexcept
{
  std::function<void (const QString&)> output_message{[this](const QString& message)
  {
    emit outputMessage(message);
  }};

  // System initialization
  outputMessage(QStringLiteral("System initialization is ... "));
  system_ = new System{settings};
  outputMessage(QStringLiteral("                                      done."));

  // Scene initialization
  outputMessage(QStringLiteral("Scene initialization is ... "));
  scene_ = new Scene{*system_, settings, output_message};
  outputMessage(QStringLiteral("                                      done."));

  // Rendering method
  const auto& world = scene_->world();
  rgb_sampler_ = makeRgbSampler<3>();
  const auto color_settings = SceneValue::toObject(settings, keyword::color);
  wavelength_sampler_ =
      makeWavelengthSampler<CoreConfig::wavelengthSampleSize()>(world, color_settings);
  const auto rendering_method_settings =
      SceneValue::toObject(settings, keyword::renderingMethod);
  rgb_rendering_method_ =
      makeRenderingMethod<3>(*system_, rendering_method_settings);
  rendering_method_ =
      makeRenderingMethod<CoreConfig::wavelengthSampleSize()>(*system_, rendering_method_settings);

  // ToneMapping
  tone_mapping_operator_ = makeToneMappingOperator(*system_, color_settings);

  // Images
  outputMessage(QStringLiteral("Create a HDR image buffer."));
  const auto& camera = scene_->camera();
  // HDR image
  hdr_image_ = new HdrImage{camera.widthResolution(), camera.heightResolution()};
  const auto hdr_byte = zisc::toMegaByte(hdr_image_->bufferMemorySize());
  const auto hdr_byte_string = QLocale{QLocale::English}.toString(hdr_byte, 'f', 3);
  outputMessage(
      QStringLiteral("    HDR image buffer size: %1 MB.").arg(hdr_byte_string));
}

/*!
  \details
  No detailed.
  */
void CpuSceneRenderer::render(const quint64 /* cycle */) noexcept
{
  auto& sampler = system_->globalSampler();
  if (system_->isRgbRenderingMode()) {
    const auto sampled_wavelengths = rgb_sampler_(sampler);
    rgb_rendering_method_->render(*system_, *scene_, sampled_wavelengths);
  }
  else {
    const auto sampled_wavelengths = wavelength_sampler_(sampler);
    rendering_method_->render(*system_, *scene_, sampled_wavelengths);
  }
}

/*!
  \details
  No detailed.
  */
void CpuSceneRenderer::toneMap() noexcept
{
  auto& ldr_image = renderedImage();
  tone_mapping_operator_->map(*system_, *hdr_image_, &ldr_image);
}

} // namespace nanairo
