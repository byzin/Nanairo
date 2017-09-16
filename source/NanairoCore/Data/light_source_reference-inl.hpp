/*!
  \file light_source_reference-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_SOURCE_REFERENCE_INL_HPP
#define NANAIRO_LIGHT_SOURCE_REFERENCE_INL_HPP

#include "light_source_reference.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class Object;

/*!
  \details
  No detailed.
  */
inline
Float LightSourceReference::inverseWeight() const noexcept
{
  return inverse_weight_;
}

/*!
  */
inline
bool LightSourceReference::isValid() const noexcept
{
  return (object() != nullptr) && (inverseWeight() != 0.0);
}

/*!
  \details
  No detailed.
  */
inline
const Object* LightSourceReference::object() const noexcept
{
  return light_source_;
}

/*!
  \details
  No detailed.
  */
inline
Float LightSourceReference::weight() const noexcept
{
  ZISC_ASSERT(isValid(), "The reference is invalid.");
  return zisc::invert(inverse_weight_);
}

} // namespace nanairo

#endif // NANAIRO_LIGHT_SOURCE_REFERENCE_INL_HPP
