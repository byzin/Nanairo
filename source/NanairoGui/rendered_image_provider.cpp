/*!
  \file rendered_image_provider.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rendered_image_provider.hpp"
// Qt
#include <QImage>
#include <QPixmap>
#include <QSize>
#include <QString>
#include <QQuickImageProvider>

namespace nanairo {

/*!
  \details
  No detailed.
  */
RenderedImageProvider::RenderedImageProvider() noexcept :
    QQuickImageProvider(QQuickImageProvider::Pixmap),
    image_{nullptr}
{
}

/*!
  \details
  No detailed.
  */
QPixmap RenderedImageProvider::requestPixmap(const QString& /* id */,
                                             QSize* size,
                                             const QSize& requested_size) noexcept
{
  const QPixmap image = (image_ != nullptr)
      ? QPixmap::fromImage(*image_)
      : QPixmap{(requested_size != QSize{-1, -1}) ? requested_size : QSize{32, 32}};
  *size = image.size();
  return image;
}

} // namespace nanairo
