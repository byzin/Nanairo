/*!
  \file object.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "object.hpp"
// Standard C++ library
#include <memory>
#include <string>
#include <string_view>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/Shape/shape.hpp"
#include "NanairoCore/Material/material.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Object::Object(zisc::UniqueMemoryPointer<Shape>&& shape,
               const Material* material) noexcept :
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
std::string_view Object::name() const noexcept
{
#ifdef Z_DEBUG_MODE
  return std::string_view{name_};
#else // Z_DEBUG_MODE
  return std::string_view{"Object"};
#endif // Z_DEBUG_MODE
}

/*!
  */
void Object::setName(const std::string_view& object_name) noexcept
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
