/*!
  \file object.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

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
Object::Object(const Material& material, UniquePointer<Geometry>&& geometry) noexcept :
    material_{material},
    geometry_{std::move(geometry)}
{
}

/*!
  \details
  No detailed.
  */
Object::Object(Object&& object) noexcept :
    material_{object.material_},
    geometry_{std::move(object.geometry_)}
{
}

} // namespace nanairo
