/*!
  \file rendered_image_provider.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rendered_image_provider.hpp"
// Qt
#include <QImage>
#include <QSize>
#include <QString>
#include <QQuickImageProvider>

namespace nanairo {

/*!
  \details
  No detailed.
  */
RenderedImageProvider::RenderedImageProvider() noexcept :
    QQuickImageProvider(QQuickImageProvider::Image),
    image_{256, 256, QImage::Format_RGB32}
{
}

/*!
  \details
  No detailed.
  */
QImage RenderedImageProvider::requestImage(
    const QString& /* id */,
    QSize* size,
    const QSize& /* requested_size */) noexcept
{
  *size = image_.size();
  return image_;
}

} // namespace nanairo
