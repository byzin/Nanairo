/*!
  \file light_source_reference-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_SOURCE_REFERENCE_INL_HPP
#define NANAIRO_LIGHT_SOURCE_REFERENCE_INL_HPP

#include "light_source_reference.hpp"
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
  return weight_;
}

} // namespace nanairo

#endif // NANAIRO_LIGHT_SOURCE_REFERENCE_INL_HPP
