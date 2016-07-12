/*!
  \file cpu_scene_renderer.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cpu_scene_renderer.hpp"
// Standard C++ library
#include <cstddef>
#include <functional>
// Qt
#include <QLocale>
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
#include "NanairoCore/ToneMapping/tone_mapping_method.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"
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
  const auto& camera_event = cameraEvent();
  auto& camera = scene_->camera();
  auto& matrix = cameraMatrix();

  // Transform camera
  if (camera_event.isTranslationEventOccured())
    matrix = matrix * camera.translate(camera_event.translation());
  else if (camera_event.isDistanceEventOccured())
    matrix = matrix * camera.distance(camera_event.distance());
  else if (camera_event.isRotationEventOccured())
    matrix = matrix * camera.rotate(camera_event.rotation());
  
  // Initialize memory
  auto& film = scene_->film();
  auto& buffer = film.spectraBuffer();
  buffer.clear();
  rgb_rendering_method_->clear();
  rendering_method_->clear();
}

/*!
  \details
  No detailed.
  */
void CpuSceneRenderer::initializeRenderer(const SceneSettings& settings) noexcept
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
  wavelength_sampler_ = makeWavelengthSampler<kWavelengthSampleSize>(settings, world);
  rgb_rendering_method_ = makeRenderingMethod<3>(*system_, settings);
  rendering_method_ = makeRenderingMethod<kWavelengthSampleSize>(*system_, settings);

  // ToneMapping
  tone_mapping_method_ = makeToneMappingMethod(*system_, settings);

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
  tone_mapping_method_->toneMap(*system_, *hdr_image_, ldr_image);
}

} // namespace nanairo
