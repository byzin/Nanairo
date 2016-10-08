/*!
  \file tone_mapping_operator-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TONE_MAPPING_OPERATOR_INL_HPP
#define NANAIRO_TONE_MAPPING_OPERATOR_INL_HPP

#include "tone_mapping_operator.hpp"
// Qt
#include <QColor>
#include <QImage>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
Float ToneMappingOperator::exposure() const noexcept
{
  return exposure_;
}

/*!
  \details
  No detailed.
  */
inline
Float ToneMappingOperator::inverseGamma() const noexcept
{
  return inverse_gamma_;
}

/*!
  \details
  No detailed.
  */
inline
QRgb* ToneMappingOperator::getPixelArray(QImage* ldr_image) const noexcept
{
  const unsigned char* bits = ldr_image->constBits();
  return zisc::treatAs<QRgb*>(const_cast<unsigned char*>(bits));
}

} // namespace nanairo

#endif // NANAIRO_TONE_MAPPING_OPERATOR_INL_HPP
