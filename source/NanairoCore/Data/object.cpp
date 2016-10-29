/*!
  \file object.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "object.hpp"
// Nanairo
#include "NanairoCore/Shape/shape.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Object::Object(const Material& material, UniquePointer<Shape>&& shape) noexcept :
    material_{material},
    shape_{std::move(shape)}
{
}

/*!
  \details
  No detailed.
  */
Object::Object(Object&& object) noexcept :
    material_{object.material_},
    shape_{std::move(object.shape_)}
{
}

} // namespace nanairo
