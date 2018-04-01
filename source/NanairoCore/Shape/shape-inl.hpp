/*!
  \file shape-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SHAPE_INL_HPP
#define NANAIRO_SHAPE_INL_HPP

#include "shape.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Float Shape::surfaceArea() const noexcept
{
  return surface_area_;
}

} // namespace nanairo

#endif // NANAIRO_SHAPE_INL_HPP
