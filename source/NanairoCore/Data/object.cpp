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
#include <utility>
// Nanairo
#include "NanairoCore/Shape/shape.hpp"
#include "NanairoCore/Material/material.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Object::Object(const Material& material, std::unique_ptr<Shape>&& shape) noexcept :
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
