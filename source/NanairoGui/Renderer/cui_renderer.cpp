/*!
  \file cui_renderer.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cui_renderer.hpp"
// Standard C++ library
#include <string>
// Qt
#include <QImage>
#include <QScopedPointer>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "simple_renderer.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/ldr_image.hpp"

namespace nanairo {

/*!
  */
CuiRenderer::CuiRenderer() noexcept
{
  initialize();
}

/*!
  */
void CuiRenderer::initLdrImageHelper() noexcept
{
  const auto& ldr_image = ldrImage();
  auto image = new QImage{zisc::treatAs<const uint8*>(&(ldr_image.data()[0])),
                          zisc::cast<int>(ldr_image.widthResolution()),
                          zisc::cast<int>(ldr_image.heightResolution()),
                          QImage::Format_RGB32};
  ldr_image_helper_.reset(image);
}

/*!
  */
void CuiRenderer::outputLdrImage(const std::string& output_path,
                                 const uint64 cycle) noexcept
{
  const auto ldr_path = makeImagePath(output_path, cycle);
  const auto& ldr_image_helper = ldrImageHelper();
  const bool result = ldr_image_helper.save(QString{ldr_path.c_str()});
  if (!result)
    logMessage("QImage error: saving image failed: " + ldr_path);
}

/*!
  */
void CuiRenderer::initialize() noexcept
{
}

} // namespace nanairo
