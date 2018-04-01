/*!
  \file film.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "film.hpp"
// Standard C++ library
#include <memory>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/rgb_spectra_image.hpp"
#include "NanairoCore/Color/spectra_image.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/camera_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Film::Film(const System& system, const SettingNodeBase* settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
void Film::initialize(const System& system, const SettingNodeBase* /* settings */) noexcept
{
  using zisc::cast;

  // Image resolution
  const auto width = system.imageWidthResolution();
  const auto height = system.imageHeightResolution();
  inverse_resolution_[0] = zisc::invert(cast<Float>(width));
  inverse_resolution_[1] = zisc::invert(cast<Float>(height));

  // Initialize buffer
  switch (system.colorMode()) {
   case RenderingColorMode::kRgb: {
    spectra_buffer_ = std::make_unique<RgbSpectraImage>(width, height);
    break;
   }
   case RenderingColorMode::kSpectra: {
    spectra_buffer_ = std::make_unique<SpectraImage>(width, height);
    break;
   }
   default: {
    zisc::raiseError("SystemError: Unsupported color mode is specified.");
    break;
   }
  }
}

} // namespace nanairo
