/*!
  \file nanairo_renderer_config.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_NANAIRO_RENDERER_CONFIG_HPP
#define NANAIRO_NANAIRO_RENDERER_CONFIG_HPP

namespace nanairo {

//! \addtogroup Renderer
//! \{

/*!
  */
class RendererConfig
{
 public:
  //! Return the max FPS
  static constexpr int maxFps() noexcept;
};

//! \} Renderer

} // namespace nanairo

#include "NanairoRenderer/nanairo_renderer_config-inl.hpp"

#endif // NANAIRO_NANAIRO_RENDERER_CONFIG_HPP
