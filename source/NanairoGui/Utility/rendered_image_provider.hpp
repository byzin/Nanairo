/*!
  \file rendered_image_provider.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_RENDERED_IMAGE_PROVIDER_HPP_
#define _NANAIRO_RENDERED_IMAGE_PROVIDER_HPP_

// Qt
#include <QImage>
#include <QQuickImageProvider>

// Forward declaration
class QSize;
class QString;

namespace nanairo {

//! \addtogroup Gui
//! \{

/*!
  \details
  No detailed.
  */
class RenderedImageProvider : public QQuickImageProvider
{
 public:
  //! Initialize image provider
  RenderedImageProvider();


  //! Return the rendered image
  QImage requestImage(const QString& id, 
                      QSize* size, 
                      const QSize& requested_size) override;

  //! Set image
  void setImage(const QImage* image);

 private:
  const QImage* image_;
};

//! \} Gui

} // namespace nanairo

#include "rendered_image_provider-inl.hpp"

#endif // _NANAIRO_RENDERED_IMAGE_PROVIDER_HPP_
