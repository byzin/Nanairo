/*!
  \file object.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_OBJECT_HPP
#define NANAIRO_OBJECT_HPP

// Standard C++ library
#include <memory>
#include <vector>
// Nanairo
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Shape/shape.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class Object
{
 public:
  //! Create object
  Object(const Material& material, std::unique_ptr<Shape>&& shape) noexcept;

  //! Move object
  Object(Object&& object) noexcept;


  //! Get shape
  const Shape& shape() const noexcept;

  //! Get material
  const Material& material() const noexcept;

 private:
  Material material_;
  std::unique_ptr<Shape> shape_;
};

//! Check if two objects are same
bool isSameObject(const Object* object1, const Object* object2) noexcept;

//! \} Core

} // namespace nanairo

#include "object-inl.hpp"

#endif // NANAIRO_OBJECT_HPP
