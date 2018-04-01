/*!
  \file tone_mapping_operator-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TONE_MAPPING_OPERATOR_INL_HPP
#define NANAIRO_TONE_MAPPING_OPERATOR_INL_HPP

#include "tone_mapping_operator.hpp"
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

} // namespace nanairo

#endif // NANAIRO_TONE_MAPPING_OPERATOR_INL_HPP
