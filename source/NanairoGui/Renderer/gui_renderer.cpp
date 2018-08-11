/*!
  \file gui_renderer.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_renderer.hpp"
// Standard C++ library
#include <cstddef>
#include <cstring>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
// Qt
#include <QObject>
#include <QString>
// Zisc
#include "zisc/error.hpp"
#include "zisc/stopwatch.hpp"
// Nanairo
#include "camera_event.hpp"
#include "cui_renderer.hpp"
#include "simple_renderer.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/scene.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"
#include "NanairoCore/CameraModel/film.hpp"
#include "NanairoCore/Color/ldr_image.hpp"

namespace nanairo {

/*!
  */
GuiRenderer::GuiRenderer(const RenderingMode mode) noexcept :
    mode_{mode}
{
  initialize();
}

/*!
  */
void GuiRenderer::handleCameraEvent(zisc::Stopwatch* stopwatch,
                                    uint32* cycle,
                                    Clock::duration* time) noexcept
{
  auto& camera_event = cameraEvent();
  if (camera_event.isEventOccured()) {
    auto& camera = scene().camera();
    // Transform camera
    if (camera_event.isHorizontalTranslationEventOccured()) {
      const auto value = camera_event.flushHorizontalTranslationEvent();
      camera.translateHorizontally(value);
    }
    if (camera_event.isVerticalTranslationEventOccured()) {
      const auto value = camera_event.flushVerticalTranslationEvent();
      camera.translateVertically(value);
    }
    if (camera_event.isRotationEventOccured()) {
      const auto value = camera_event.flushRotationEvent();
      camera.rotate(value);
    }
    // Reset rendering info
    initForRendering();
    ZISC_ASSERT(stopwatch != nullptr, "The stopwatch is null.");
    ZISC_ASSERT(cycle != nullptr, "The cycle is null.");
    ZISC_ASSERT(time != nullptr, "The time is null.");
    stopwatch->stop();
    stopwatch->start();
    *cycle = 0;
    *time = Clock::duration::zero();
  }
}

/*!
  */
void GuiRenderer::initialize() noexcept
{
  if (mode_ == RenderingMode::kPreviewing)
    enableSavingAtEachCycle(true);
}

/*!
  */
void GuiRenderer::initLogger(const std::string& output_path,
                             std::ostream*,
                             std::ofstream* text_log_stream) noexcept
{
  CuiRenderer::initLogger(output_path, nullptr, text_log_stream);
}

/*!
  */
void GuiRenderer::notifyOfRenderingInfo(const std::string_view& info) const noexcept
{
  QString information{info.data()};
  notifiedOfRenderingInfo(information);
}

/*!
  */
void GuiRenderer::outputLdrImage(const std::string_view output_path,
                                 const uint32 cycle,
                                 const std::string_view suffix) noexcept
{
  const auto& ldr_image_helper = ldrImageHelper();

  // Copy image
  const auto& ldr_image = ldrImage();
  auto data = const_cast<uint8*>(ldr_image_helper.constBits());
  const std::size_t memory_size = sizeof(ldr_image[0]) * ldr_image.size();
  std::memcpy(data, ldr_image.data().data(), memory_size);

  if (mode_ == RenderingMode::kRendering) {
    const auto ldr_path = makeImagePath(output_path, cycle, suffix);

    const bool result = ldr_image_helper.save(QString{ldr_path.c_str()});
    if (!result)
      logMessage("QImage error: saving image failed: " + ldr_path);
  }
}

} // namespace nanairo
