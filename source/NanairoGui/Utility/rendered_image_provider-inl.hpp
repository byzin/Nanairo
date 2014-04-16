/*!
  \file rendered_image_provider-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_RENDERED_IMAGE_PROVIDER_INL_HPP_
#define _NANAIRO_RENDERED_IMAGE_PROVIDER_INL_HPP_

#include "rendered_image_provider.hpp"
// Qt
#include <QImage>

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
void RenderedImageProvider::setImage(const QImage* image)
{
  image_ = image;
}

} // namespace nanairo

#endif // _NANAIRO_RENDERED_IMAGE_PROVIDER_INL_HPP_
