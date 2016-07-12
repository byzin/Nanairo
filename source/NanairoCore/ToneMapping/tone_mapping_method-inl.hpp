/*!
  \file tone_mapping_method-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TONEMAPPING_METHOD_INL_HPP
#define NANAIRO_TONEMAPPING_METHOD_INL_HPP

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
QRgb* ToneMappingMethod::getPixelArray(QImage& ldr_image) noexcept
{
  const unsigned char* bits = ldr_image.constBits();
  return zisc::treatAs<QRgb*>(const_cast<unsigned char*>(bits));
}

/*!
  \details
  No detailed.
  */
inline
Float ToneMappingMethod::inverseGamma() const noexcept
{
  return inverse_gamma_;
}

/*!
  \details
  No detailed.
  */
inline
const Matrix3x3& ToneMappingMethod::toRgbMatrix() const noexcept
{
  return to_rgb_matrix_;
}

/*!
  \details
  No detailed.
  */
inline
const Matrix3x3& ToneMappingMethod::toXyzMatrix() const noexcept
{
  return to_xyz_matrix_;
}

} // namespace nanairo

#endif // NANAIRO_TONEMAPPING_METHOD_INL_HPP
