/*!
  \file gui_renderer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_GUI_RENDERER_INL_HPP
#define NANAIRO_GUI_RENDERER_INL_HPP

#include "gui_renderer.hpp"
// Nanairo
#include "camera_event.hpp"

namespace nanairo {

/*!
  */
inline
CameraEvent& GuiRenderer::cameraEvent() noexcept
{
  return camera_event_;
}

/*!
  */
inline
const CameraEvent& GuiRenderer::cameraEvent() const noexcept
{
  return camera_event_;
}

} // namespace nanairo

#endif // NANAIRO_GUI_RENDERER_INL_HPP
