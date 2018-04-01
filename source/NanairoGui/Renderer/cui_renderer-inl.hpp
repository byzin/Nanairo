/*!
  \file cui_renderer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CUI_RENDERER_INL_HPP
#define NANAIRO_CUI_RENDERER_INL_HPP

#include "cui_renderer.hpp"
// Qt
#include <QImage>
#include <QScopedPointer>
// Zisc
#include "zisc/error.hpp"

namespace nanairo {

/*!
  */
inline
const QImage& CuiRenderer::ldrImageHelper() const noexcept
{
  ZISC_ASSERT(!ldr_image_helper_.isNull(), "The image helper is null.");
  return *ldr_image_helper_;
}

} // namespace nanairo

#endif // NANAIRO_CUI_RENDERER_INL_HPP
