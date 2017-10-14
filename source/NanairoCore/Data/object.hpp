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
#include <string>
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
  Object(std::unique_ptr<Shape>&& shape, const Material* material) noexcept;

  //! Move object data
  Object(Object&& other) noexcept;


  //! Move object data
  Object& operator=(Object&& ohter) noexcept;


  //! Return the name of the object
  const std::string* name() const noexcept;

  //! Get material
  const Material& material() const noexcept;

  //! Set the name of the object
  void setName(const std::string& object_name) noexcept;

  //! Get shape
  const Shape& shape() const noexcept;

  //! Swap object data
  void swap(Object& other) noexcept;

 private:
  std::unique_ptr<Shape> shape_;
  const Material* material_;
#ifdef Z_DEBUG_MODE
  std::string name_;
#endif // Z_DEBUG_MODE
};

//! Check if two objects are same
bool isSameObject(const Object* object1, const Object* object2) noexcept;

//! Swap object data
void swap(Object& lhs, Object& rhs) noexcept;

//! \} Core

} // namespace nanairo

#include "object-inl.hpp"

#endif // NANAIRO_OBJECT_HPP
