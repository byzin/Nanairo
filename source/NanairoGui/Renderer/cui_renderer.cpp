/*!
  \file cui_renderer.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cui_renderer.hpp"
// Standard C++ library
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
// Qt
#include <QImage>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "simple_renderer.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/ldr_image.hpp"

namespace nanairo {

/*!
  */
CuiRenderer::CuiRenderer() noexcept :
    ldr_image_helper_{nullptr}
{
  initialize();
}

/*!
  */
QImage& CuiRenderer::ldrImageHelper() noexcept
{
  ZISC_ASSERT(ldr_image_helper_ != nullptr, "The image is null.");
  return *ldr_image_helper_;
}

/*!
  */
const QImage& CuiRenderer::ldrImageHelper() const noexcept
{
  ZISC_ASSERT(ldr_image_helper_ != nullptr, "The image is null.");
  return *ldr_image_helper_;
}

/*!
  */
void CuiRenderer::setImage(QImage* image) noexcept
{
  ldr_image_helper_ = image;
}

/*!
  */
void CuiRenderer::outputLdrImage(const std::string_view output_path,
                                 const uint32 cycle,
                                 const std::string_view suffix) noexcept
{
  ZISC_ASSERT(ldr_image_helper_ != nullptr, "The image is null.");

  // Copy image
  const auto& ldr_image = ldrImage();
  auto data = const_cast<uint8*>(ldr_image_helper_->constBits());
  const std::size_t memory_size = sizeof(ldr_image[0]) * ldr_image.size();
  std::memcpy(data, ldr_image.data().data(), memory_size);

  const auto ldr_path = makeImagePath(output_path, cycle, suffix);

  const bool result = ldr_image_helper_->save(QString{ldr_path.c_str()});
  if (!result)
    logMessage("QImage error: saving image failed: " + ldr_path);
}

/*!
  */
void CuiRenderer::initialize() noexcept
{
}

} // namespace nanairo
