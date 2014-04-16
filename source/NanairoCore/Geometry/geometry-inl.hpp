/*!
  \file geometry-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_GEOMETRY_INL_HPP_
#define _NANAIRO_GEOMETRY_INL_HPP_

#include "geometry.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Float Geometry::surfaceArea() const
{
  return surface_area_;
}

} // namespace nanairo

#endif // _NANAIRO_GEOMETRY_INL_HPP_
