/*!
  \file film.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "film.hpp"
// Standard C++ library
#include <utility>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/rgb_spectra_image.hpp"
#include "NanairoCore/Color/spectra_image.hpp"
#include "NanairoCore/Utility/scene_value.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Film::Film(const System& system, const QJsonObject& settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
void Film::initialize(const System& system, const QJsonObject& /* settings */) noexcept
{
  using zisc::cast;

  // Image resolution
  const auto width = system.imageWidthResolution();
  const auto height = system.imageHeightResolution();
  inverse_width_ = zisc::invert(cast<Float>(width));
  inverse_height_ = zisc::invert(cast<Float>(height));

  // Initialize buffer
  if (system.isRgbRenderingMode())
    spectra_buffer_ = new RgbSpectraImage{width, height};
  else
    spectra_buffer_ = new SpectraImage{width, height};
}

} // namespace nanairo
