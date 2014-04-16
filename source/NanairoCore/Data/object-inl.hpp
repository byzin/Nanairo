/*!
  \file object-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_OBJECT_INL_HPP_
#define _NANAIRO_OBJECT_INL_HPP_

#include "object.hpp"
// Nanairo
#include "NanairoCore/Geometry/geometry.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
const Geometry& Object::geometry() const
{
  return *geometry_;
}

/*!
  \details
  No detailed.
  */
inline
const Material& Object::material() const
{
  return material_;
}

/*!
  \details
  No detailed.
  */
inline
bool isSameObject(const Object* object1, const Object* object2)
{
  return object1 == object2;
}

} // namespace nanairo

#endif // _NANAIRO_OBJECT_INL_HPP_
