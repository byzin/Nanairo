/*!
  \file object-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_OBJECT_INL_HPP
#define NANAIRO_OBJECT_INL_HPP

#include "object.hpp"
// Nanairo
#include "NanairoCore/Shape/shape.hpp"
#include "NanairoCore/Material/material.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
const Shape& Object::shape() const noexcept
{
  return *shape_;
}

/*!
  \details
  No detailed.
  */
inline
const Material& Object::material() const noexcept
{
  return *material_;
}

/*!
  \details
  No detailed.
  */
inline
bool isSameObject(const Object* object1, const Object* object2) noexcept
{
  return object1 == object2;
}

} // namespace nanairo

#endif // NANAIRO_OBJECT_INL_HPP
