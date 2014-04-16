/*!
  \file light_source_reference-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_LIGHT_SOURCE_REFERENCE_INL_HPP_
#define _NANAIRO_LIGHT_SOURCE_REFERENCE_INL_HPP_

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
Float LightSourceReference::inverseWeight() const
{
  return inverse_weight_;
}

/*!
  \details
  No detailed.
  */
inline
const Object* LightSourceReference::object() const
{
  return light_source_;
}

/*!
  \details
  No detailed.
  */
inline
Float LightSourceReference::weight() const
{
  return weight_;
}

} // namespace nanairo

#endif // _NANAIRO_LIGHT_SOURCE_REFERENCE_INL_HPP_
