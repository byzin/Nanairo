/*!
  \file tone_mapping_method-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_TONEMAPPING_METHOD_INL_HPP_
#define _NANAIRO_TONEMAPPING_METHOD_INL_HPP_

#include "tone_mapping_method.hpp"
// Qt
#include <QColor>
#include <QImage>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
QRgb* ToneMappingMethod::getPixelArray(QImage& ldr_image)
{
  const unsigned char* bits = ldr_image.constBits();
  return zisc::treatAs<QRgb*>(const_cast<unsigned char*>(bits));
}

/*!
  \details
  No detailed.
  */
inline
Float ToneMappingMethod::inverseGamma() const
{
  return inverse_gamma_;
}

/*!
  \details
  No detailed.
  */
inline
const Matrix3x3& ToneMappingMethod::toRgbMatrix() const
{
  return to_rgb_matrix_;
}

/*!
  \details
  No detailed.
  */
inline
const Matrix3x3& ToneMappingMethod::toXyzMatrix() const
{
  return to_xyz_matrix_;
}

} // namespace nanairo

#endif // _NANAIRO_TONEMAPPING_METHOD_INL_HPP_
