/*!
  \file gui_renderer.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_renderer.hpp"
// Standard C++ library
#include <string>
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
                                    uint64* cycle,
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
void GuiRenderer::logMessage(const std::string&) noexcept
{
}

/*!
  */
void GuiRenderer::notifyOfRenderingInfo(const std::string& info) const noexcept
{
  QString information{info.c_str()};
  notifiedOfRenderingInfo(information);
}

/*!
  */
void GuiRenderer::outputLdrImage(const std::string& output_path,
                                 const uint64 cycle) noexcept
{
  if (mode_ == RenderingMode::kRendering) {
    const auto ldr_path = makeImagePath(output_path, cycle);
    const auto& ldr_image_helper = ldrImageHelper();
    ldr_image_helper.save(QString{ldr_path.c_str()});
  }
}

} // namespace nanairo
