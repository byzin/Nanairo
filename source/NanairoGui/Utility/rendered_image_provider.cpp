/*!
  \file rendered_image_provider.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
    image_{nullptr}
{
}

/*!
  \details
  No detailed.
  */
QImage RenderedImageProvider::requestImage(const QString& /* id */,
                                           QSize* size,
                                           const QSize& /* requested_size */) noexcept
{
  if (image_ == nullptr) {
    QSize image_size{640, 480};
    QImage image{image_size, QImage::Format_RGB32};
    *size = image_size;
    return image;
  }
  *size = image_->size();
  return *image_;
}

} // namespace nanairo
