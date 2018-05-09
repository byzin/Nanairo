/*!
  \file rendered_image_provider-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RENDERED_IMAGE_PROVIDER_INL_HPP
#define NANAIRO_RENDERED_IMAGE_PROVIDER_INL_HPP

#include "rendered_image_provider.hpp"
// Qt
#include <QImage>

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
QImage& RenderedImageProvider::image() noexcept
{
  return image_;
}

/*!
  */
inline
const QImage& RenderedImageProvider::image() const noexcept
{
  return image_;
}

} // namespace nanairo

#endif // NANAIRO_RENDERED_IMAGE_PROVIDER_INL_HPP
