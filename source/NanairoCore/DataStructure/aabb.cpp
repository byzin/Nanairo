/*!
  \file aabb.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "aabb.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Aabb::Aabb() noexcept
{
}

/*!
  \details
  No detailed.
  */
Aabb::Aabb(const Point3& min, const Point3& max) noexcept :
    point_{min, max}
{
}

/*!
  \details
  No detailed.
  */
void Aabb::setMaxPoint(const Point3& point) noexcept
{
  point_[1] = point;
}

/*!
 \details
 No detailed.
 */
void Aabb::setMinPoint(const Point3& point) noexcept
{
  point_[0] = point;
}

} // namespace nanairo
