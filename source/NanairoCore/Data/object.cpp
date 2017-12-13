/*!
  \file object.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "object.hpp"
// Standard C++ library
#include <memory>
#include <string>
#include <utility>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/Shape/shape.hpp"
#include "NanairoCore/Material/material.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Object::Object(std::unique_ptr<Shape>&& shape, const Material* material) noexcept :
    shape_{std::move(shape)},
    material_{material}
{
  ZISC_ASSERT(material != nullptr, "The material is null.");
}

/*!
  \details
  No detailed.
  */
Object::Object(Object&& other) noexcept :
    material_{nullptr}
{
  swap(other);
}

/*!
  */
Object& Object::operator=(Object&& other) noexcept
{
  swap(other);
  return *this;
}

/*!
  */
const std::string* Object::name() const noexcept
{
  const std::string* object_name = nullptr;
#ifdef Z_DEBUG_MODE
  object_name = &name_;
#endif // Z_DEBUG_MODE
  return object_name;
}

/*!
  */
void Object::setName(const std::string& object_name) noexcept
{
#ifdef Z_DEBUG_MODE
  name_ = object_name;
#endif // Z_DEBUG_MODE
  static_cast<void>(object_name);
}

/*!
  */
void Object::swap(Object& other) noexcept
{
  // Shape
  {
    auto tmp = std::move(other.shape_);
    other.shape_ = std::move(shape_);
    shape_ = std::move(tmp);
  }
  // Material
  {
    auto tmp = other.material_;
    other.material_ = material_;
    material_ = tmp;
  }
#ifdef Z_DEBUG_MODE
  // Name
  {
    auto tmp = std::move(other.name_);
    other.name_ = std::move(name_);
    name_ = std::move(tmp);
  }
#endif // Z_DEBUG_MODE
}

/*!
  */
void swap(Object& lhs, Object& rhs) noexcept
{
  lhs.swap(rhs);
}

} // namespace nanairo
